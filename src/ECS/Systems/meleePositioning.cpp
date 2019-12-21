#include "meleePositioning.hpp"
#include "meleePositioning.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Logs/logs.hpp"
#include "SFML/System/Vector2.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void MeleePositioning::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto playerView = mRegistry.view<component::MeleeAttacker, component::Player, component::BodyRect, component::FaceDirection>();
	for (auto player : playerView)
	{
		const auto& [playerFaceDirection, meleeAttacker, playerBody] 
			= playerView.get<component::FaceDirection, component::MeleeAttacker, component::BodyRect>(player);

		auto meleeView = mRegistry.view<component::CurrentMeleeWeapon, component::BodyRect, component::RenderQuad>();
		for (auto melee : meleeView)
		{
			auto& [currentMelee, body, renderQuad] 
				= meleeView.get<component::CurrentMeleeWeapon, component::BodyRect, component::RenderQuad>(melee);

			// set weapon position
			const sf::Vector2f playerCenter = playerBody.rect.getCenter();

			if(playerFaceDirection.direction == PH_EAST)
				body.rect.setPosition(playerCenter + sf::Vector2f(0.f, -3.f));
			else if(playerFaceDirection.direction == PH_WEST)
				body.rect.setPosition(playerCenter + sf::Vector2f(3.f, -3.f));
			else if(playerFaceDirection.direction == PH_NORTH)
				body.rect.setPosition(playerCenter + sf::Vector2f(-3.f, 1.f));
			else if(playerFaceDirection.direction == PH_SOUTH)
				body.rect.setPosition(playerCenter + sf::Vector2f(-3.f, 9.f));
			else if(playerFaceDirection.direction == PH_NORTH_WEST)
				body.rect.setPosition(playerCenter + sf::Vector2f(0.f, -8.f));
			else if(playerFaceDirection.direction == PH_NORTH_EAST)
				body.rect.setPosition(playerCenter + sf::Vector2f(0.f, -8.f));
			else if(playerFaceDirection.direction == PH_SOUTH_WEST)
				body.rect.setPosition(playerCenter + sf::Vector2f(3.f, -3.f));
			else if(playerFaceDirection.direction == PH_SOUTH_EAST)
				body.rect.setPosition(playerCenter + sf::Vector2f(-3.f, -1.f));
			else
				body.rect.setPosition(playerCenter);

			constexpr float anglesPerSecond = 240.f;

			currentMelee.rotation += dt * anglesPerSecond;
			renderQuad.rotation = -currentMelee.rotation;

			if (!meleeAttacker.isAttacking)
				mRegistry.assign_or_replace<component::HiddenForRenderer>(melee);
			else
				if (mRegistry.has<component::HiddenForRenderer>(melee))
					mRegistry.remove<component::HiddenForRenderer>(melee);
		}
	}
}

}
