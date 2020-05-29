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
		// change spikes texture
		if(spikes.active)
			spikesRenderQuad.texture = spikesTexture;
		else
			spikesRenderQuad.texture = notActiveSpikesTexture;

		// spikes hurt player
		if(spikes.active)
		{
			mRegistry.view<component::Player, component::BodyRect, component::BodyCircle>().each([&]
			(auto playerEntity, auto, const auto& playerBody, auto playerCircle)
			{
				if(Math::intersect(spikesBody, playerBody.pos + playerCircle.offset, playerCircle.radius))
				{
					mRegistry.assign_or_replace<component::DamageTag>(playerEntity, 1);
				}
			});
		}

		// change spikes activeness over type
		if(spikes.changes)
		{
			spikes.timeToChange -= dt;
			if(spikes.timeToChange < 0.f)
			{
				spikes.timeToChange = spikes.changeFrequency;
				spikes.active = !spikes.active;
			}
		}
	});
}

}

