#pragma once

#include <entt/entity/registry.hpp>

namespace ph {

	class Xml;
	class EntitiesTemplateStorage;
	class SceneManager;

	void loadEntitiesFromMapFile(const Xml& mapNode, EntitiesTemplateStorage& templatesStorage, entt::registry& gameRegistry,
	                             SceneManager& sceneManager, bool* isPlayerOnScene);
}

