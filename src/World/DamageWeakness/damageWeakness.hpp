#pragma once

#include "damageWeaknessType.hpp"

namespace ph{

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

}
