#pragma once
#include<vector>
#include <string>
#include "src/DamagePipeline/DamageValue.h"
#include "src/Weapons/StatusEffect.h"
#include "src/Target/Faction.h"

class DamageInstanceModEffectInterface{
	public:
	virtual std::vector<DamageValue> GetDamageData() = 0;
	virtual void AddDamageValue(DamageValue damageValue) = 0;
	virtual std::vector<StatusEffect> GetStatusEffects() = 0;
	virtual void AddStatusEffect(StatusEffect statusEffect) = 0;

	virtual std::string GetAttackName() = 0;

	virtual std::string GetWeaponCategory() = 0;
	virtual float GetFireRate() = 0;
	virtual float GetCriticalChance() = 0;
	virtual float GetCriticalDamage() = 0;
	virtual float GetCriticalTier() = 0;
	virtual float GetStatusChance() = 0;
	virtual float GetStatusDamageMultiplier() = 0;

	virtual int GetModSetCount(std::string setName) = 0;

	virtual int GetTargetLevel() = 0;
	virtual float GetTargetMaxHealth() = 0;
	virtual float GetTargetCurrentHealth() = 0;
	virtual Faction GetTargetFaction() = 0;
	virtual bool IsTargetHitzoneWeakPoint() = 0;
	virtual bool IsTargetHitzoneHeadshot() = 0;
	virtual float GetTargetHitzoneMultiplier() = 0;
	virtual float GetTargetArmour() = 0;
	virtual std::vector<StatusEffect> GetStatusEffectsOnTarget() = 0;
};