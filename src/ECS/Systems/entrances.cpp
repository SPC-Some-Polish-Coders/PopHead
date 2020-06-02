#include "pch.hpp"
#include "entrances.hpp"
#include "Scenes/sceneManager.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/entityUtil.hpp"

namespace ph::system {

Entrances::Entrances(entt::registry& registry, SceneManager& sceneManager)
	:System(registry)
	,mSceneManager(sceneManager)
{
}

using namespace component;

void Entrances::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause) return;

	auto playerCenterPos = getPlayerCenterPos();

	mRegistry.view<Entrance, BodyRect>().each([&]
	(const auto& entrance, auto entranceBody)
	{
		if(entranceBody.contains(playerCenterPos))
		{
			std::string sceneFilepath = "scenes/" + entrance.entranceDestination.substr(2);
			mSceneManager.replaceScene(sceneFilepath, entrance.playerSpawnPosition);
			return;
		}
	});
}

}
