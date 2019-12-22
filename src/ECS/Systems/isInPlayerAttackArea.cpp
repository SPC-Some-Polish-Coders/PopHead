#include "isInPlayerAttackArea.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Renderer/renderer.hpp"

#include "Utilities/math.hpp"
#include "Utilities/profiling.hpp"
#include "Utilities/direction.hpp"

namespace ph::system {

void IsInPlayerAttackArea::update(float dt)
{
	PH_PROFILE_FUNCTION();

	handleMelee();
}



void IsInPlayerAttackArea::handleMelee() const
{
	auto playerView = mRegistry.view<component::Player, component::BodyRect, component::FaceDirection>();
	auto enemiesView = mRegistry.view<component::Killable, component::BodyRect>();
	auto currentMeleeView = mRegistry.view<component::CurrentMeleeWeapon, component::MeleeProperties>();
	for (auto player : playerView)
	{
		const auto& [playerBody, faceDirection] = playerView.get<component::BodyRect, component::FaceDirection>(player);
		for (auto currentMelee : currentMeleeView)
		{
			const auto& meleeProperties = currentMeleeView.get<component::MeleeProperties>(currentMelee);

			FloatRect attackArea(playerBody.rect.getCenter(), sf::Vector2f(0.f, 0.f));

			attackArea.width, attackArea.height =  meleeProperties.range * 2.f;
			attackArea.left -= attackArea.width / 2.f;
			attackArea.top -= attackArea.height / 2.f;

			//for (auto enemy : enemiesView)
			//{
			//	const auto& enemyBody = enemiesView.get<component::BodyRect>(enemy);
			//	if (attackArea.doPositiveRectsIntersect(enemyBody.rect))
			//		mRegistry.assign_or_replace<component::InPlayerAttackArea>(enemy);
			//}


		}
	}
}

}