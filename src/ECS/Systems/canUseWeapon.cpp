#include "canUseWeapon.hpp"

#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

void CanUseGun::update(float seconds)
{
	auto gunAttackerView = mRegistry.view<component::GunAttacker>();

	for (const auto& gunAttacker : gunAttackerView)
	{
		auto& gunAttackerComp = gunAttackerView.get<component::GunAttacker>(gunAttacker);

		if (gunAttackerComp.cooldownSinceLastShoot > 0.f)
			gunAttackerComp.cooldownSinceLastShoot -= seconds;

		if (gunAttackerComp.isTryingToAttack)
		{
			gunAttackerComp.canAttack = gunAttackerComp.cooldownSinceLastShoot <= 0.f && gunAttackerComp.bullets > 0;

			if(gunAttackerComp.canAttack)
				gunAttackerComp.cooldownSinceLastShoot = gunAttackerComp.minSecondsInterval;
		}
	}
}

void CanUseMelee::update(float seconds)
{
	auto meleeAttackerView = mRegistry.view<component::MeleeAttacker>();

	for (const auto& meleeAttacker : meleeAttackerView)
	{
		auto& gunAttackerComp = meleeAttackerView.get<component::MeleeAttacker>(meleeAttacker);

		if (gunAttackerComp.cooldownSinceLastHit > 0.f)
			gunAttackerComp.cooldownSinceLastHit -= seconds;

		if (gunAttackerComp.isTryingToAttack)
		{
			gunAttackerComp.canAttack = gunAttackerComp.cooldownSinceLastHit <= 0.f;

			if (gunAttackerComp.canAttack)
				gunAttackerComp.cooldownSinceLastHit = gunAttackerComp.minSecondsInterval;
		}
	}
}

}
