#include "pch.hpp"
#include "pits.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

namespace ph::system {

using namespace component;

void Pits::update(float dt)
{
	PH_PROFILE_FUNCTION();

	mRegistry.view<PitChunk, InsideSimRegion, BodyRect>().each([&]
	(const auto& pitChunk, auto, auto pitChunkBody)
	{
		// NOTE: KinematicCollisionBody is here to make player not fall into pits when we're using pf (player flying) terminal command
		mRegistry.view<BodyRect, BodyCircle, KinematicCollisionBody, InsideSimRegion> 
		(entt::exclude<IsOnPlatform, CurrentlyDashing, FallingIntoPit>).each([&]
		(auto objectEntity, auto objectRect, auto objectCircle, auto, auto)
		{
			if(intersect(pitChunkBody, objectRect))
			{
				for(const auto& pitBody : pitChunk.pits)
				{
					if(intersect(pitBody, objectRect, objectCircle))
					{
						mRegistry.assign<FallingIntoPit>(objectEntity);
						if(auto* rq = mRegistry.try_get<RenderQuad>(objectEntity))
							rq->z = 188;
						return;
					}
				}
			}
		});
	});

	mRegistry.view<FallingIntoPit, BodyRect>().each([&]
	(auto entity, auto& falling, auto& body)
	{
		falling.timeToEnd -= dt;
		body.setSizeWithFixedCenter(Vec2(20.f) * falling.timeToEnd);
		if(falling.timeToEnd < 0.f)
		{
			DamageTag tag{1000, false};
			mRegistry.assign_or_replace<DamageTag>(entity, tag);
			mRegistry.remove<FallingIntoPit>(entity);
		}
	});
}

}
