#include "canUseWeapon.hpp"

#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

void CanUseWeapon::update(float seconds)
{
	updateMeleeWeapon(seconds);
	updateGun(seconds);
}

void CanUseWeapon::updateMeleeWeapon(float dt)
{
	auto meleeAttackerView = mRegistry.view<component::MeleeAttacker>();

	for (const auto& meleeAttacker : meleeAttackerView)
	{
		auto& gunAttackerComp = meleeAttackerView.get<component::MeleeAttacker>(meleeAttacker);

		if (gunAttackerComp.cooldownSinceLastHit > 0.f)
			gunAttackerComp.cooldownSinceLastHit -= dt;

		if (gunAttackerComp.isTryingToAttack)
		{
			gunAttackerComp.canAttack = gunAttackerComp.cooldownSinceLastHit <= 0.f;

			if (gunAttackerComp.canAttack)
				gunAttackerComp.cooldownSinceLastHit = gunAttackerComp.minSecondsInterval;
		}
	}
}

void CanUseWeapon::updateGun(float dt)
{
	auto gunAttackerView = mRegistry.view<component::GunAttacker>();

	for (const auto& gunAttacker : gunAttackerView)
	{
		auto& gunAttackerComp = gunAttackerView.get<component::GunAttacker>(gunAttacker);

		if (gunAttackerComp.cooldownSinceLastShoot > 0.f)
			gunAttackerComp.cooldownSinceLastShoot -= dt;

		if (gunAttackerComp.isTryingToAttack)
		{
			gunAttackerComp.canAttack = gunAttackerComp.cooldownSinceLastShoot <= 0.f && gunAttackerComp.bullets > 0;

			if (gunAttackerComp.canAttack)
				gunAttackerComp.cooldownSinceLastShoot = gunAttackerComp.minSecondsInterval;
		}
	}
}

}
