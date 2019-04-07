#include "damageWeakness.hpp"

using PopHead::World::DamageWeakness;

DamageWeakness::DamageWeakness(DamageWeaknessType type, float additionalDamage)
:mType(type)
,mAdditionalDamage(additionalDamage)
{
}

DamageWeakness::operator==(DamageWeakness&)
{

}

auto DamageWeakness::getType() const -> DamageWeaknessType
{
    return mType;
}
