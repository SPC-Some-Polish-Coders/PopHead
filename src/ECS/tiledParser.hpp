#pragma once

#include "entitiesTemplateStorage.hpp"
#include "Scenes/sceneManager.hpp"
#include "Utilities/rect.hpp"
#include <entt/entity/registry.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

namespace ph {

	class Xml;

	class TiledParser
	{
	public:
		TiledParser(EntitiesTemplateStorage& templatesStorage, entt::registry& gameRegistry,
		            SceneManager& sceneManager);

		void parseFile(const Xml& mapNode) const;
		bool hasLoadedPlayer() const { return mHasLoadedPlayer; }

	private:
		Xml findGameObjects(const Xml& mapFile) const;
		void loadObjects(const Xml& gameObjects) const;

		void createDebugName(entt::entity, const char* name) const;
		
		void loadZombie(const Xml& entityNode, std::string zombieTypeName = "Zombie") const;
		void loadLootSpawner(const Xml& entityNode) const;
		void loadArcadeSpawner(const Xml& entityNode) const;
		void loadEntrance(const Xml& entityNode) const;
		void loadVelocityChangingArea(const Xml& entityNode) const;
		void loadPushingArea(const Xml& entityNode) const;
		void loadHintArea(const Xml& velocityChangingAreaNode) const;
		std::optional<std::string> getSceneFileName(const std::string& scenePathRelativeToMapFile) const;
		void loadGate(const Xml& entityNode) const;
		void loadLever(const Xml& entityNode) const;
		void loadCar(const Xml& entityNode) const;
		void loadCamera(const Xml& entityNode) const;
		void loadPlayer(const Xml& entityNode) const;
		void loadBulletBox(const Xml& entityNode) const;
		void loadMedkit(const Xml& entityNode) const;
		void loadSprite(const Xml& entityNode) const;
		void loadTorch(const Xml& entityNode) const;
		void loadLightWall(const Xml& entityNode) const;
		void loadFlowingRiver(const Xml& entityNode) const;
		void loadIndoorOutdoorBlendArea(const Xml& entityNode) const;

		void loadHealthComponent(const Xml& entityNode, entt::entity entity) const;
		void loadPosition(const Xml& entityNode, entt::entity entity) const;
		void loadSize(const Xml& entityNode, entt::entity entity) const;
		void loadPositionAndSize(const Xml& entityNode, entt::entity entity) const;
		void loadPositionAndOptionalSize(const Xml& entityNode, entt::entity entity) const;

		Xml getProperty(const Xml& objectNode, const std::string& propertyName) const;
		bool hasCustomProperty(const Xml& gameObjectNode, const std::string& propertyName) const;
		Xml getCustomProperties(const Xml& gameObjectNode, const std::string& name) const;
		Xml getDefaultProperties(const std::string& name, const std::string& propertyName) const;
		std::vector<Xml> getObjectTypeNodes() const;

		sf::Vector2f getPositionAttribute(const Xml& gameObjectNode) const;
		sf::Vector2f getSizeAttribute(const Xml& gameObjectNode) const;
		std::optional<sf::Vector2f> getOptionalSizeAttribute(const Xml& gameObjectNode) const;

	private:
		EntitiesTemplateStorage& mTemplatesStorage;
		entt::registry& mGameRegistry;
		SceneManager& mSceneManager;
		mutable bool mHasLoadedPlayer = false;
	};

}
