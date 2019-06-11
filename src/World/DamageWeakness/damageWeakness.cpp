#include "damageWeakness.hpp"

ph::DamageWeakness::DamageWeakness(DamageWeaknessType type, float additionalDamage)
	:mType(type)
	,mAdditionalDamage(additionalDamage)
{
}

bool ph::DamageWeakness::operator==(DamageWeakness&)
{
	return true;
}

auto ph::DamageWeakness::getType() const -> DamageWeaknessType { return mType; }
