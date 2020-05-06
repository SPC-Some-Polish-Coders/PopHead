#include "pch.hpp"
#include "entrances.hpp"
#include "Scenes/sceneManager.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

Entrances::Entrances(entt::registry& registry, SceneManager& sceneManager)
	:System(registry)
	,mSceneManager(sceneManager)
{
}

void Entrances::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	auto playerView = mRegistry.view<component::Player, component::BodyRect>();
	auto entrancesView = mRegistry.view<component::Entrance, component::BodyRect>();

	for(auto player : playerView)
	{
		const auto& playerBody = playerView.get<component::BodyRect>(player);

		for(auto entrance : entrancesView)
		{
			const auto& entranceBody = entrancesView.get<component::BodyRect>(entrance);
			if(entranceBody.contains(playerBody.center()))
			{
				const auto& entranceDetails = entrancesView.get<component::Entrance>(entrance);
				std::string sceneFilepath = "scenes/" + entranceDetails.entranceDestination.substr(2);
				mSceneManager.replaceScene(sceneFilepath, entranceDetails.playerSpawnPosition);
				return;
			}
		}
	}
}

}
