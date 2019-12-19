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
	auto entrancesView = mRegistry.view<component::Entrance, component::BodyRect>();
	auto playerView = mRegistry.view<component::Player, component::BodyRect>();

	for (auto player : playerView)
	{
		auto& playerBody = playerView.get<component::BodyRect>(player);

		for (auto entrance : entrancesView)
		{
			const auto& entranceBody = entrancesView.get<component::BodyRect>(entrance);
			if (entranceBody.rect.contains(playerBody.rect.getCenter()))
			{
				const auto& entranceDetails = entrancesView.get<component::Entrance>(entrance);
				mSceneManager.replaceScene(entranceDetails.entranceDestination, entranceDetails.playerSpawnPosition);
				return;
			}
		}
	}
}

}
