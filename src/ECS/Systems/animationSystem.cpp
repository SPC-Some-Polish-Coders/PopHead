#include "animationSystem.hpp"
#include "ECS/Components/animationComponents.h"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph::system {

void AnimationSystem::update(float seconds)
{
	auto view = mRegistry.view<component::AnimationData, component::TextureRect>();

	for(auto entity : view)
	{
		auto& animationData = view.get<component::AnimationData>(entity);
		auto& textureRect = view.get<component::TextureRect>(entity);
		
		if(animationData.isPlaying)
		{
			animationData.elapsedTime += seconds;
			while(animationData.elapsedTime >= animationData.delay)
			{
				animationData.elapsedTime -= animationData.delay;
				StateData& state = animationData.states->at(animationData.currentStateName);
				if(++animationData.currentFrameIndex == state.framesCount)
					animationData.currentFrameIndex = 0;
				textureRect.rect = IntRect(
					state.startFrame.left + state.startFrame.width * animationData.currentFrameIndex,
					state.startFrame.top,
					state.startFrame.width,
					state.startFrame.height
				);
			}
		}
		else 
		{
			StateData& state = animationData.states->at(animationData.currentStateName);
			animationData.currentFrameIndex = 0;
			textureRect.rect = IntRect(
				state.startFrame.left,
				state.startFrame.top,
				state.startFrame.width,
				state.startFrame.height
			);
		}
	}
}

}
