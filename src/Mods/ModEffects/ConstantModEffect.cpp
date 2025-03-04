#include "src/Mods/ModEffects/ConstantModEffect.h"

ConstantModEffect::ConstantModEffect(DamageType damageType, ModUpgradeType upgradeType, ModOperationType operationType, float value)
{
	_damageType = damageType;
	_upgradeType = upgradeType;
	_operationType = operationType;
	_value = value;
}

ConstantModEffect::~ConstantModEffect()
{

}

DamageType ConstantModEffect::GetDamageType()
{
	return _damageType;
}

ModOperationType ConstantModEffect::GetModOperationType()
{
	return _operationType;
}

ModUpgradeType ConstantModEffect::GetUpgradeType()
{
	return _upgradeType;
}

float ConstantModEffect::GetModValue(shared_ptr<DamageInstanceModEffectInterface> damageInstance)
{
	(void)damageInstance;
	return _value;
}

float ConstantModEffect::GetAverageModValue(shared_ptr<DamageInstanceModEffectInterface> damageInstance)
{
	(void)damageInstance;
	return _value;
}
