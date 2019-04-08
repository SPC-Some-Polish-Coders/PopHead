#include "damageWeakness.hpp"

using PopHead::World::DamageWeakness;
using PopHead::World::DamageWeaknessType;

DamageWeakness::DamageWeakness(DamageWeaknessType type, float additionalDamage)
:mType(type)
,mAdditionalDamage(additionalDamage)
{
}

bool DamageWeakness::operator==(DamageWeakness&)
{
	return true;
}

auto DamageWeakness::getType() const -> DamageWeaknessType { return mType; }
