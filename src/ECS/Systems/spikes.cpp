#include "pch.hpp"
#include "spikes.hpp"

#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"

namespace ph::system {

void Spikes::update(float dt)
{
	mRegistry.view<component::Spikes, component::BodyRect>().each([this]
	(component::Spikes, const component::BodyRect& spikesBody)
	{
		mRegistry.view<component::Player, component::BodyRect>().each([&]
		(auto playerEntity, component::Player, const component::BodyRect& playerBody)
		{
			if(intersect(playerBody, spikesBody))
			{
				mRegistry.assign_or_replace<component::DamageTag>(playerEntity, 1);
			}
		});
	});
}

}
