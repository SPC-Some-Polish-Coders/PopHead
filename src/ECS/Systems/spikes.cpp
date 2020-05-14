#include "pch.hpp"
#include "spikes.hpp"
#include "Resources/textureHolder.hpp"
#include "Renderer/API/texture.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

static Texture* spikesTexture;
static Texture* notActiveSpikesTexture;

void Spikes::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(!spikesTexture)
	{
		spikesTexture = &getTexture("textures/others/spike.png");
		loadTexture("textures/others/spikesNotActive.png");
		notActiveSpikesTexture = &getTexture("textures/others/spikesNotActive.png");
	}

	mRegistry.view<component::Spikes, component::RenderQuad, component::BodyRect>().each([&]
	(auto& spikes, auto& spikesRenderQuad, const auto& spikesBody)
	{
		if(spikes.active)
		{
			mRegistry.view<component::Player, component::BodyRect>().each([&]
			(auto playerEntity, auto, const auto& playerBody)
			{
				if(intersect(playerBody, spikesBody))
				{
					mRegistry.assign_or_replace<component::DamageTag>(playerEntity, 1);
				}
			});
		}

		if(spikes.changes)
		{
			spikes.timeToChange -= dt;
			if(spikes.timeToChange < 0.f)
			{
				spikes.timeToChange = spikes.changeFrequency;
				spikes.active = !spikes.active;
				spikesRenderQuad.texture = spikes.active ? spikesTexture : notActiveSpikesTexture; 
			}
		}
	});
}

}
