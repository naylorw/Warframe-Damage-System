#pragma once

class ModUpgradeType
{
public:
	enum ModUpgradeTypeEnum
	{
		WEAPON_DAMAGE_AMOUNT,
		WEAPON_PERCENT_BASE_DAMAGE_ADDED,
		WEAPON_MULTISHOT,
		WEAPON_FIRE_RATE,
		WEAPON_RECOIL,
		WEAPON_SPREAD,
		WEAPON_AMMO_MAXIMUM,
		WEAPON_MAGAZINE_CAPACITY,
		WEAPON_RELOAD_SPEED,
		WEAPON_PUNCH_THROUGH,
		WEAPON_CRIT_CHANCE,
		WEAPON_CRIT_DAMAGE,
		WEAPON_CRIT_TIER,
		WEAPON_STATUS_CHANCE,
		WEAPON_STATUS_DAMAGE,
		WEAPON_FACTION_DAMAGE,
		WEAPON_WEAKPOINT_DAMAGE,
		WEAPON_ADD_STATUS,
		WEAPON_DAMAGE_AMOUNT_IF_VICTIM_PROC_ACTIVE,
		WEAPON_PROJECTILE_SPEED,
		WEAPON_DAMAGE_MULTIPLIER,
		WARFRAME_ABILITY_STRENGTH,
		WARFRAME_ABILITY_DURATION,
		WARFRAME_ABILITY_EFFICIENCY,
		WARFRAME_ABILITY_RANGE,
	};

	ModUpgradeType() = default;
	constexpr ModUpgradeType(ModUpgradeTypeEnum aModUpgradeType) : _modUpgradeType(aModUpgradeType) {}

	// Allow switch and comparisons.
	constexpr operator ModUpgradeTypeEnum() const { return _modUpgradeType; }

	// Prevent usage: if(modupgradetype)
	explicit operator bool() const = delete;
	
	

private:
	ModUpgradeTypeEnum _modUpgradeType;
};