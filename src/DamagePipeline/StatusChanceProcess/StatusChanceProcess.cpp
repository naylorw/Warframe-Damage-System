#include "src/DamagePipeline/StatusChanceProcess/StatusChanceProcess.h"
#include "src/DamagePipeline/DamagePipeline.h"

#define DEBUG_STATUS_PROCESS false
#if DEBUG_STATUS_PROCESS
#include "src/Services/ServiceLocator.h"
#endif

void StatusChanceProcess::EvaluateStatusChanceProcess(shared_ptr<DamageInstance> damageInstance)
{
	EvaluateStatusChanceMods(damageInstance);
	EvaluateStatusDamageMods(damageInstance);
	EvaluateStatusDurationMods(damageInstance);
	RollForStatus(damageInstance);
}

void StatusChanceProcess::EvaluateStatusChanceMods(shared_ptr<DamageInstance> damageInstance)
{
	if (!damageInstance->moddedStatusChance.needsToBeCalculated)
	{
		// These mods have already been evaluated, no need to solve again
		return;
	}

	// Calculate the base status chance per pellet
	float baseStatusChance = damageInstance->damageData.statusChance / damageInstance->damageData.multishot;
	damageInstance->moddedStatusChance.Set(DamagePipeline::EvaluateAndApplyModEffects(damageInstance, ModUpgradeType::WEAPON_STATUS_CHANCE, baseStatusChance));

#if DEBUG_STATUS_PROCESS
	ServiceLocator::GetLogger().Log("Modded status chance: " + std::to_string(damageInstance->moddedStatusChance.Get()));
#endif
}

void StatusChanceProcess::EvaluateStatusDamageMods(shared_ptr<DamageInstance> damageInstance)
{
	if (!damageInstance->moddedStatusDamageMultiplier.needsToBeCalculated)
	{
		// These mods have already been evaluated, no need to solve again
		return;
	}

	damageInstance->moddedStatusDamageMultiplier.Set(DamagePipeline::EvaluateAndApplyModEffects(damageInstance, ModUpgradeType::WEAPON_STATUS_DAMAGE, 1));

#if DEBUG_STATUS_PROCESS
	ServiceLocator::GetLogger().Log("Modded status damage: " + std::to_string(damageInstance->moddedStatusDamageMultiplier.Get()));
#endif
}

void StatusChanceProcess::EvaluateStatusDurationMods(shared_ptr<DamageInstance> damageInstance)
{
	if (!damageInstance->moddedStatusDurationMultiplier.needsToBeCalculated)
	{
		// These mods have already been evaluated, no need to solve again
		return;
	}

	damageInstance->moddedStatusDurationMultiplier.Set(DamagePipeline::EvaluateAndApplyModEffects(damageInstance, ModUpgradeType::WEAPON_STATUS_DURATION, 1));
#if DEBUG_STATUS_PROCESS
	ServiceLocator::GetLogger().Log("Modded status duration: " + std::to_string(damageInstance->moddedStatusDurationMultiplier.Get()));
#endif
}

void StatusChanceProcess::RollForStatus(shared_ptr<DamageInstance> damageInstance)
{
	// Copy the elemental weights into a new map that may be modified
	auto elementalWeights{damageInstance->GetElementalWeights()};

	// If the target is immune to the status effect, set the weight of it to 0
	for (auto damageTypeWeightPair : elementalWeights)
	{
		for (ProcType pt : damageInstance->target->immuneStatusEffects)
		{
			if (ProcType::GetProcTypeFromElement(damageTypeWeightPair.first) == pt)
			{
				elementalWeights[damageTypeWeightPair.first] = 0;
			}
		}
	}

	// Calculate the sum of the elemental weights to allow a weighted RNG generation
	float totalElementalWeighting = 0;
	for (auto damageTypeWeightPair : elementalWeights)
	{
		totalElementalWeighting += damageTypeWeightPair.second;
	}

	if (damageInstance->calculateAverageDamage)
	{
		AverageRollForStatus(damageInstance, elementalWeights, totalElementalWeighting);
	}
	else
	{
		NormalRollForStatus(damageInstance, elementalWeights, totalElementalWeighting);
	}

	// Add forced procs innate from the weapon
	for (ProcType forcedProc : damageInstance->damageData.forcedProcs)
	{
#if DEBUG_STATUS_PROCESS
		ServiceLocator::GetLogger().Log("Applying forced status effect: " + forcedProc.ToString());
#endif
		damageInstance->AddStatusEffect(StatusEffect(forcedProc, damageInstance, damageInstance->baseDamageValue));
	}
}

void StatusChanceProcess::NormalRollForStatus(shared_ptr<DamageInstance> damageInstance, std::map<DamageType, float> &elementalWeights, float totalElementalWeighting)
{
	// Roll the number of statuses from status chance
	int numberOfStatuses = ServiceLocator::GetRNG().WeightedFloorCeiling(damageInstance->GetStatusChance());
#if DEBUG_STATUS_PROCESS
	ServiceLocator::GetLogger().Log("Rolled number of statuses: " + std::to_string(numberOfStatuses));
#endif

	for (int j = 0; j < numberOfStatuses; j++)
	{
		float randomNumber = ServiceLocator::GetRNG().RandomFloat(0, totalElementalWeighting);

		float counter = 0;
		for (auto damageTypeWeightPair : elementalWeights)
		{
			counter += damageTypeWeightPair.second;
			if (counter > randomNumber)
			{
#if DEBUG_STATUS_PROCESS
				ServiceLocator::GetLogger().Log("Applying status effect: " + ProcType::GetProcTypeFromElement(damageTypeWeightPair.first).ToString());
#endif
				auto statusEffectProcType = ProcType::GetProcTypeFromElement(damageTypeWeightPair.first);
				damageInstance->AddStatusEffect(StatusEffect(statusEffectProcType, damageInstance, damageInstance->baseDamageValue));
				break;
			}
		}
	}
}

void StatusChanceProcess::AverageRollForStatus(shared_ptr<DamageInstance> damageInstance, std::map<DamageType, float> &elementalWeights, float totalElementalWeighting)
{
	// For the average case, apply all status effects with their damage scaled by the probability that they will trigger per bullet
	for (std::pair<DamageType, float> damageTypeWeightPair : elementalWeights)
	{
		float probabilityOfProc = damageTypeWeightPair.second / totalElementalWeighting * damageInstance->GetStatusChance();
#if DEBUG_STATUS_PROCESS
		ServiceLocator::GetLogger().Log("For Damage Type: " + damageTypeWeightPair.first.ToString() + ", probability of proc per shot = " + std::to_string(probabilityOfProc));
#endif
		auto statusEffectProcType = ProcType::GetProcTypeFromElement(damageTypeWeightPair.first);
		damageInstance->AddStatusEffect(StatusEffect(statusEffectProcType, damageInstance, damageInstance->baseDamageValue * probabilityOfProc));
	}
}
