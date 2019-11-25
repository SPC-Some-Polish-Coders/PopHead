#include "gunAttackerSystem.hpp"

#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

void GunAttackerSystem::update(float seconds)
{
	auto gunAttackerView = mRegistry.view<component::GunAttacker>();

	for (const auto& gunAttacker : gunAttackerView)
	{
		auto& gunAttackerComp = gunAttackerView.get<component::GunAttacker>(gunAttacker);

		if (gunAttackerComp.cooldownSinceLastShoot > 0.f)
			gunAttackerComp.cooldownSinceLastShoot -= seconds;

		if (gunAttackerComp.isTryingToAttack)
		{
			bool canAttack = true;

			if (gunAttackerComp.cooldownSinceLastShoot > 0.f || gunAttackerComp.bullets <= 0)
				canAttack = false;

			gunAttackerComp.canAttack = canAttack;	

			if(canAttack)
				gunAttackerComp.cooldownSinceLastShoot = gunAttackerComp.minSecondsInterval;
		}
	}
}

}
