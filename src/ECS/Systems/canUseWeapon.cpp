#include "canUseWeapon.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void CanUseWeapon::update(float dt)
{
	PH_PROFILE_FUNCTION();

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
