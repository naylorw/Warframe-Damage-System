#include "src/Mods/ModEffects/ProbabilityModEffectWrapper.h"
#include "src/Services/ServiceLocator.h"
#include "ProbabilityModEffectWrapper.h"

ProbabilityModEffect::ProbabilityModEffect(ModEffectBase& wrapped, float probabilityOfSucess) : _wrapped(wrapped)
{
	_wrapped = wrapped;
	_probabilityOfSuccess = probabilityOfSucess;
}

ProbabilityModEffect::~ProbabilityModEffect()
{
}

DamageType ProbabilityModEffect::GetDamageType()
{
	return _wrapped.GetDamageType();
}

ModOperationType ProbabilityModEffect::GetModOperationType()
{
	return _wrapped.GetModOperationType();
}

ModUpgradeType ProbabilityModEffect::GetUpgradeType()
{
	return _wrapped.GetUpgradeType();
}

float ProbabilityModEffect::GetModValue(DamageInstanceModEffectInterface *damageInstance)
{
	bool success = ServiceLocator::GetRNG().WeightedRandomBool(_probabilityOfSuccess);

	if (success){
		return _wrapped.GetModValue(damageInstance);
	}
	else {
		return 0.0f;
	}
}

float ProbabilityModEffect::GetAverageModValue(DamageInstanceModEffectInterface *damageInstance)
{
	return _probabilityOfSuccess * _wrapped.GetAverageModValue(damageInstance);
}
