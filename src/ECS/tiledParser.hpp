#pragma once

#include "entitiesTemplateStorage.hpp"
#include "Scenes/sceneManager.hpp"

#include <entt/entity/registry.hpp>

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

namespace ph {

	class GameData;
	class GameObject;
	class CutSceneManager;
	class Xml;

	class TiledParser
	{
	public:
		TiledParser(CutSceneManager& cutSceneManager, EntitiesTemplateStorage& templatesStorage, entt::registry& gameRegistry, SceneManager& sceneManager);

		void parseFile(const std::string& filePath) const;

	private:
		Xml findGameObjects(const Xml& mapFile) const;
		
		void loadObjects(const Xml& gameObjects) const;
		void loadLayerObjects() const;
		void loadContainerObjects() const;
		void loadArcadeManager() const;
		//bool isObjectOfType(const Xml& gameObjectNode, const std::string& typeName) const;
		
		void loadSlowZombie(const Xml& SlowZombieNode) const;
		void loadZombie(const Xml& zombieNode) const;
		void loadNpc(const Xml& npcNode) const;
		void loadSpawner(const Xml& spawnerNode) const;
		void loadLootSpawner(const Xml& lootSpawnerNode) const;
		void loadArcadeSpawner(const Xml& arcadeSpawnerNode) const;
		void loadEntrance(const Xml& entranceNode) const;
		void loadSlowDownArea(const Xml& slowDownAreaNode) const;
		void loadActivateArea(const Xml& openGateAreaNode) const;
		void loadCutSceneArea(const Xml& cutSceneAreaNode) const;
		std::optional<std::string> getSceneFileName(const std::string& scenePathRelativeToMapFile) const;
		void loadGate(const Xml& gateNode) const;
		void loadLever(const Xml& leverNode) const;
		void loadCar(const Xml& carNode) const;
		void loadCamera(const Xml& cameraNode) const;
		void loadPlayer(const Xml& playerNode) const;
		void loadCutScene(const Xml& cutSceneNode) const;
		void loadCrawlingNpc(const Xml& crawlingNpcNode) const;
		void loadGateGuardNpc(const Xml& gateGuardNpcNode) const;
		void loadBulletItem(const Xml& bulletItemNode) const;
		void loadMedkit(const Xml& bulletItemNode) const;
		void loadSpriteNode(const Xml& spriteNodeNode) const;

		Xml getProperty(const Xml& objectNode, const std::string& propertyName) const;
		bool hasCustomProperty(const Xml& gameObjectNode, const std::string& propertyName) const;
		Xml getCustomProperties(const Xml& gameObjectNode, const std::string& name) const;
		Xml getDefaultProperties(const std::string& name, const std::string& propertyName) const;
		std::vector<Xml> getObjectTypeNodes() const;

		sf::Vector2f getPositionAttribute(const Xml& gameObjectNode) const;
		sf::Vector2f getSizeAttribute(const Xml& gameObjectNode) const;

	private:
		CutSceneManager& mCutSceneManager;
		EntitiesTemplateStorage& mTemplatesStorage;
		entt::registry& mGameRegistry;
		SceneManager& mSceneManager;
		mutable bool mHasLoadedPlayer;
	};

}
