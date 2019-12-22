#include "levers.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Events/actionEventManager.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void Levers::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto leverView = mRegistry.view<component::Lever, component::BodyRect, component::TextureRect>();

	for (auto player : playerView)
	{
		const auto& playerBody = playerView.get<component::BodyRect>(player);
		for (auto lever : leverView)
		{
			const auto& [leverBody, leverTexture] = leverView.get<component::BodyRect, component::TextureRect>(lever);
			auto& leverDetails = leverView.get<component::Lever>(lever);

			if (leverDetails.activationCooldown > 0.f)
			{
				leverDetails.activationCooldown -= dt;
				return;
			}

			if (leverDetails.turnOffAfterSwitch && leverDetails.isActivated)
				continue;

			if (leverBody.rect.doPositiveRectsIntersect(playerBody.rect))
				if (ActionEventManager::isActionPressed("use"))
				{
					leverDetails.isActivated = !leverDetails.isActivated;
					leverDetails.activationCooldown = leverDetails.minActivationInterval;
					leverTexture.rect = leverDetails.isActivated ? IntRect(9, 0, 7, 15) : IntRect(0, 0, 7, 15);
					handleListeners(leverDetails.isActivated, leverDetails.id);
				}
		}
	}
}

void Levers::handleListeners(bool isActivated, unsigned leverId) const
{
	auto listenersView = mRegistry.view<component::LeverListener>();
	for (auto leverListener : listenersView)
	{
		auto& listenerDetails = listenersView.get<component::LeverListener>(leverListener);
		if (listenerDetails.observedLeverId == leverId)
			listenerDetails.isActivated = true;
	}
}

}

