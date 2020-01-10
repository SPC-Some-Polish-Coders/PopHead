#include "animationSystem.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void AnimationSystem::update(float dt)
{
	PH_PROFILE_FUNCTION(0);

	auto view = mRegistry.view<component::AnimationData, component::TextureRect>();

	for(auto entity : view)
	{
		auto& [animationData, textureRect] = view.get<component::AnimationData, component::TextureRect>(entity);
		
		if(animationData.isPlaying)
		{
			animationData.elapsedTime += dt;
			while(animationData.elapsedTime >= animationData.delay)
			{
				animationData.elapsedTime -= animationData.delay;
				StateData& state = animationData.states->at(animationData.currentStateName);
				if(++animationData.currentFrameIndex >= state.frameCount)
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
