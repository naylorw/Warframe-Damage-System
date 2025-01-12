#include "src/Mods/ModFactory.h"
#include "src/Mods/ModEffects/ModEffect.h"

Mod* ModFactory::GetMod(std::string name)
{
	Mod* mod = new Mod();
	mod->name = name;
	mod->weaponClass = "Primary";
	mod->polarity = ModPolarity::NONE;
	mod->rank = 10;
	mod->maxRank = 10;
	mod->baseCapacityDrain = 4;

	DamageType modEffectElement = DamageType::ParseDamageTypeName(name);

	std::vector<ModEffectBase> modEffects = {};
	mod->AddModEffect(new ModEffect(modEffectElement, ModUpgradeType::WEAPON_PERCENT_BASE_DAMAGE_ADDED, ModOperationType::STACKING_MULTIPLY, 0.6f));
	
	return mod;
}