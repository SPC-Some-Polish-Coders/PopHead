#include "pch.hpp"
#include "spikesSystem.hpp"
#include "Resources/textureHolder.hpp"
#include "Renderer/API/texture.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

static Texture* spikesTexture;
static Texture* notActiveSpikesTexture;

using namespace component;

void SpikesSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	if(!spikesTexture)
	{
		spikesTexture = &getTexture("textures/others/spike.png");
		loadTexture("textures/others/spikesNotActive.png");
		notActiveSpikesTexture = &getTexture("textures/others/spikesNotActive.png");
	}

	mRegistry.view<Spikes, RenderQuad, BodyRect>().each([&]
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
			mRegistry.view<Player, BodyRect, BodyCircle>(entt::exclude<CurrentlyDashing>).each([&]
			(auto playerEntity, auto, auto playerBody, auto playerCircle)
			{
				if(Math::intersect(spikesBody, playerBody.pos + playerCircle.offset, playerCircle.radius))
					mRegistry.assign_or_replace<DamageTag>(playerEntity, 1);
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

