#include "pch.hpp"
#include "animationSystem.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"

namespace ph::system {

using namespace component;

void AnimationSystem::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	mRegistry.view<AnimationData, TextureRect, InsideSimRegion>().each([&]
	(auto& animationData, auto& textureRect, auto)
	{
		if(animationData.isPlaying)
		{
			animationData.elapsedTime += dt;
			while(animationData.elapsedTime >= animationData.delay)
			{
				animationData.elapsedTime -= animationData.delay;
				StateData& state = animationData.states->at(animationData.currentStateName);
				if(++animationData.currentFrameIndex >= state.frameCount)
					animationData.currentFrameIndex = 0;

				textureRect = state.startFrame;
				textureRect.x = state.startFrame.x + state.startFrame.w * animationData.currentFrameIndex;
			}
		}
		else 
		{
			StateData& state = animationData.states->at(animationData.currentStateName);
			animationData.currentFrameIndex = 0;
			textureRect = state.startFrame;
		}
	});
}

}
