#include "levers.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

#include "Events/actionEventManager.hpp"

namespace ph::system {

void Levers::update(float seconds)
{
	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto leverView = mRegistry.view<component::Lever, component::BodyRect, component::TextureRect>();
	for (auto player : playerView)
	{
		const auto& [playerInteractionInput, playerBody] = playerView.get<component::Player, component::BodyRect>(player);
		for (auto lever : leverView)
		{
			const auto& [leverBody, leverTexture] = leverView.get<component::BodyRect, component::TextureRect>(lever);
			auto& leverDetails = leverView.get<component::Lever>(lever);

			if (leverDetails.activationCooldown > 0.f)
			{
				leverDetails.activationCooldown -= seconds;
				return;
			}

			if (leverBody.rect.doPositiveRectsIntersect(playerBody.rect))
			{
				if (ActionEventManager::isActionPressed("use"))
				{
					leverDetails.isActivated = !leverDetails.isActivated;
					leverDetails.activationCooldown = leverDetails.minActivationInterval;
					leverTexture.rect = leverDetails.isActivated ? IntRect(9, 0, 7, 15) : IntRect(0, 0, 7, 15);
				}
			}
		}
	}
}

}