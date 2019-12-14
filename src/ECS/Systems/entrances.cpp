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

void Entrances::update(float seconds)
{
	auto entrancesView = mRegistry.view<component::Area, component::Entrance>();
	auto playerView = mRegistry.view<component::Player, component::BodyRect>();

	for (auto player : playerView)
	{
		const auto& playerBody = playerView.get<component::BodyRect>(player);
		for (auto entrance : entrancesView)
		{
			const auto& entranceBody = entrancesView.get<component::Area>(entrance);
			if (entranceBody.areaBody.contains(playerBody.rect.getCenter()))
			{
				const auto& entranceDetails = entrancesView.get<component::Entrance>(entrance);
				mSceneManager.replaceScene(entranceDetails.entranceDestination, entranceDetails.playerSpawnPosition);
				return;
			}
		}
	}
}

}