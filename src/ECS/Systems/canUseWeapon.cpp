#include "canUseWeapon.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"

namespace ph::system {

void CanUseWeapon::update(float seconds)
{
	updateMeleeWeapon(seconds);
	updateGun(seconds);
}

void CanUseWeapon::updateMeleeWeapon(float dt)
{
	auto meleeAttackerView = mRegistry.view<component::MeleeAttacker>();
	auto currentMeleeView = mRegistry.view<component::CurrentMeleeWeapon, component::MeleeProperties>();

	for (auto meleeAttacker : meleeAttackerView)
	{
		for (auto melee : currentMeleeView)
		{
			auto& meleeAttackerDetails = meleeAttackerView.get<component::MeleeAttacker>(meleeAttacker);
			const auto& currentMeleeProperties = currentMeleeView.get<component::MeleeProperties>(melee);

			if (meleeAttackerDetails.cooldownSinceLastHit > 0.f)
				meleeAttackerDetails.cooldownSinceLastHit -= dt;

			if (meleeAttackerDetails.isTryingToAttack)
			{
				meleeAttackerDetails.canAttack = meleeAttackerDetails.cooldownSinceLastHit <= 0.f;

				if (meleeAttackerDetails.canAttack)
					meleeAttackerDetails.cooldownSinceLastHit = currentMeleeProperties.minHitInterval;
			}
		}
	}
}

void CanUseWeapon::updateGun(float dt)
{
	auto gunAttackerView = mRegistry.view<component::GunAttacker>();
	auto currentGunView = mRegistry.view<component::CurrentGun, component::GunProperties>();

	for (const auto& gunAttacker : gunAttackerView)
	{
		for (auto currentGun : currentGunView)
		{
			auto& gunAttackerDetails = gunAttackerView.get<component::GunAttacker>(gunAttacker);
			const auto& currentGunProperties = currentGunView.get<component::GunProperties>(currentGun);

			if (gunAttackerDetails.cooldownSinceLastShoot > 0.f)
				gunAttackerDetails.cooldownSinceLastShoot -= dt;

			if (gunAttackerDetails.isTryingToAttack)
			{
				gunAttackerDetails.canAttack = gunAttackerDetails.cooldownSinceLastShoot <= 0.f && gunAttackerDetails.bullets > 0;

				if (gunAttackerDetails.canAttack)
					gunAttackerDetails.cooldownSinceLastShoot = currentGunProperties.minShotsInterval;
			}
		}
	}
}

}
