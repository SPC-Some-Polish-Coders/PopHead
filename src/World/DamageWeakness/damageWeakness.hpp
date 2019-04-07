#ifndef POPHEAD_WORLD_DAMAGEWEAKNESS_DAMAGEWEAKNESS_H_
#define POPHEAD_WORLD_DAMAGEWEAKNESS_DAMAGEWEAKNESS_H_

#include "damageWeaknessType.hpp"

namespace PopHead{
namespace World{


class DamageWeakness
{
public:
    DamageWeakness(DamageWeaknessType, float additionalDamage);
    bool operator==(DamageWeakness&);
    auto getType() const -> DamageWeaknessType;

private:
    const DamageWeaknessType mType;
    float mAdditionalDamage;
};


}}

#endif // POPHEAD_WORLD_DAMAGEWEAKNESS_DAMAGEWEAKNESS_H_
