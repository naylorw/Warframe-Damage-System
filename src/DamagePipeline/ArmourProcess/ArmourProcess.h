#pragma once
#include "src/DamagePipeline/DamagePipeline.h"
#include "src/DamagePipeline/DamageInstance.h"

class ArmourProcess
{
public:
	static void EvaluateAndApplyArmourDamageReduction(shared_ptr<DamageInstance> damageInstance);

private:
};