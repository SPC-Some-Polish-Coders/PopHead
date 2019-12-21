#include "canUseWeapon.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void CanUseWeapon::update(float dt)
{
	PH_PROFILE_FUNCTION();

	updateMeleeWeapon(dt);
	updateGun(dt);
}

void CanUseWeapon::updateMeleeWeapon(float dt)
{
	auto meleeAttackerView = mRegistry.view<component::MeleeAttacker>();
	auto currentMeleeView = mRegistry.view<component::CurrentMeleeWeapon, component::MeleeProperties>();

	for (auto meleeAttacker : meleeAttackerView)
	{
		auto& meleeAttackerDetails = meleeAttackerView.get<component::MeleeAttacker>(meleeAttacker);

		for (auto melee : currentMeleeView)
		{
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
		auto& gunAttackerDetails = gunAttackerView.get<component::GunAttacker>(gunAttacker);

		for (auto currentGun : currentGunView)
		{
			const auto& currentGunProperties = currentGunView.get<component::GunProperties>(currentGun);

			if (gunAttackerDetails.cooldownSinceLastShot > 0.f)
				gunAttackerDetails.cooldownSinceLastShot -= dt;

			if (gunAttackerDetails.isTryingToAttack)
			{
				gunAttackerDetails.canAttack = gunAttackerDetails.cooldownSinceLastShot <= 0.f && gunAttackerDetails.bullets > 0;

				if (gunAttackerDetails.canAttack)
					gunAttackerDetails.cooldownSinceLastShot = currentGunProperties.minShotsInterval;
			}
		}
	}
}

}
