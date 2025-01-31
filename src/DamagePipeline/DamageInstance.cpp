#include "src/Weapons/DamageType.h"
#include "src/DamagePipeline/DamageInstance.h"
#include "src/Weapons/WeaponFactory.h"

DamageInstance::DamageInstance()
{
	weapon = WeaponFactory::GetNullWeapon();
	attackName = "Normal Attack";
	target = new Target();
	targetBodyPart = "Body";

	damageData = weapon->data.attacks.at(attackName).attackData;
}

DamageInstance::DamageInstance(const DamageInstance &other)
{
	damageData = other.damageData;
	statusEffects = other.statusEffects;
	critTier = other.critTier;

	weapon = other.weapon;
	attackName = other.attackName;
	target = other.target;
	targetBodyPart = other.targetBodyPart;

	moddedCriticalChance = other.moddedCriticalChance;
	moddedCriticalDamage = other.moddedCriticalDamage;
	moddedStatusChance = other.moddedStatusChance;
	moddedStatusDamageMultiplier = other.moddedStatusDamageMultiplier;
}

DamageInstance::DamageInstance(Weapon &_weapon, std::string _attackName, Target &_target, std::string targetBodyPart)
{
	weapon = &_weapon;
	attackName = _attackName;
	target = &_target;
	this->targetBodyPart = targetBodyPart;

	damageData = weapon->data.attacks.at(attackName).attackData;

	if (weapon != nullptr && weapon->data.attacks.count(attackName) > 0)
	{
		moddedCriticalChance = weapon->data.attacks.at(attackName).criticalChance;
		moddedCriticalDamage = weapon->data.attacks.at(attackName).criticalDamage;
		moddedStatusChance = weapon->data.attacks.at(attackName).statusChance;
		moddedStatusDamageMultiplier = 1;
	}
	else
	{
		ServiceLocator::GetLogger().LogError("Nullptr provided to weapon field in FireInstance constructor or attack name not found.");

		attackName = weapon->data.attacks.begin()->second.attackName;
		moddedCriticalChance = 0;
		moddedCriticalDamage = 1;
		moddedStatusChance = 0;
		moddedStatusDamageMultiplier = 1;
	}
}

DamageInstance &DamageInstance::operator*(const float &mult)
{
	return *this *= mult;
}

DamageInstance &DamageInstance::operator=(const DamageInstance &other)
{
	damageData = std::vector<DamageValue>(other.damageData);
	statusEffects = std::vector<StatusEffect>(other.statusEffects);
	critTier = other.critTier;
	return *this;
}

DamageInstance &DamageInstance::operator*=(const float &mult)
{
	for (int i = 0; i < damageData.size(); i++)
	{
		damageData[i] *= mult;
	}

	return *this;
}

std::vector<ModEffectBase *> DamageInstance::GetAllModEffects(ModUpgradeType upgradeType)
{
	std::vector<ModEffectBase *> relevantEffects = {};

	auto weaponEffects = weapon->GetAllWeaponModEffects(upgradeType);
	relevantEffects.insert(relevantEffects.end(), weaponEffects.begin(), weaponEffects.end());

	for (auto mod : target->innateUpgrades)
	{
		if (mod == nullptr)
		{
			continue;
		}

		for (auto modEffect : mod->GetModEffects())
		{
			if (modEffect->GetUpgradeType() == upgradeType)
			{
				relevantEffects.push_back(modEffect);
			}
		}
	}

	// ServiceLocator::GetLogger().Log("For UpgradeType: " + std::to_string(upgradeType) + " the length of modEffects is " + std::to_string(relevantEffects.size()));

	return relevantEffects;
}

float DamageInstance::GetTotalDamage()
{
	float sum = 0;
	for (int i = 0; i < damageData.size(); i++)
	{
		sum += damageData[i].value;
	}
	return sum;
}

std::vector<DamageValue> DamageInstance::GetDamageData()
{
	return damageData;
}

void DamageInstance::AddDamageValue(DamageValue damageValue)
{
	if (damageValue.value > 0 && damageValue.damageType != DamageType::DT_ANY)
	{
		damageData.push_back(damageValue);
	}
}

std::vector<StatusEffect> DamageInstance::GetStatusEffects()
{
	return statusEffects;
}

void DamageInstance::AddStatusEffect(StatusEffect statusEffect)
{
	if (statusEffect != StatusEffect::PT_NONE)
	{
		statusEffects.push_back(statusEffect);
	}
}

std::string DamageInstance::GetAttackName()
{
	return attackName;
}

std::string DamageInstance::GetWeaponCategory()
{
	return weapon->data.weaponCategory;
}

float DamageInstance::GetFireRate()
{
	return weapon->data.attacks.at(attackName).fireRate;
}

float DamageInstance::GetCriticalChance()
{
	return moddedCriticalChance;
}

float DamageInstance::GetCriticalDamage()
{
	return moddedCriticalDamage;
}

float DamageInstance::GetCriticalTier()
{
	return critTier;
}

float DamageInstance::GetStatusChance()
{
	return moddedStatusChance;
}

float DamageInstance::GetStatusDamageMultiplier()
{
	return moddedStatusDamageMultiplier;
}

int DamageInstance::GetModSetCount(std::string setName)
{
	return 0;
}

int DamageInstance::GetTargetLevel()
{
	return target->level;
}

float DamageInstance::GetTargetMaxHealth()
{
	return target->maxHealth;
}

float DamageInstance::GetTargetCurrentHealth()
{
	return target->currentHealth;
}

Faction DamageInstance::GetTargetFaction()
{
	return target->faction;
}

bool DamageInstance::IsTargetHitzoneWeakPoint()
{
	return target->IsBodyPartWeakPoint(targetBodyPart);
}

bool DamageInstance::IsTargetHitzoneHeadshot()
{
	return targetBodyPart == "Head";
}

float DamageInstance::GetTargetHitzoneMultiplier()
{
	return target->GetBodyPartMultiplier(targetBodyPart);
}

float DamageInstance::GetTargetArmour()
{
	return target->GetArmour();
}

std::vector<StatusEffect> DamageInstance::GetStatusEffectsOnTarget()
{
	return target->afflictedStatusEffects;
}
