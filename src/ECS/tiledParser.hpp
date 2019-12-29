#pragma once

#include "entitiesTemplateStorage.hpp"
#include "Scenes/sceneManager.hpp"
#include "Utilities/rect.hpp"
#include <entt/entity/registry.hpp>
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

namespace ph {

	class CutSceneManager;
	class Xml;

	class TiledParser
	{
	public:
		TiledParser(CutSceneManager& cutSceneManager, EntitiesTemplateStorage& templatesStorage, entt::registry& gameRegistry, SceneManager& sceneManager);

		void parseFile(const std::string& filePath) const;
		bool hasLoadedPlayer() const { return mHasLoadedPlayer; }

	private:
		Xml findGameObjects(const Xml& mapFile) const;
		
		void loadObjects(const Xml& gameObjects) const;
		void loadArcadeManager() const;
		
		void loadZombie(const Xml& zombieNode, std::string zombieTypeName = "Zombie") const;
		void loadNpc(const Xml& npcNode) const;
		void loadSpawner(const Xml& spawnerNode) const;
		void loadLootSpawner(const Xml& lootSpawnerNode) const;
		void loadArcadeSpawner(const Xml& arcadeSpawnerNode) const;
		void loadEntrance(const Xml& entranceNode) const;
		void loadVelocityChangingArea(const Xml& velocityChangingAreaNode) const;
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
		void loadMedkit(const Xml& medkitItemNode) const;
		void loadSpriteNode(const Xml& spriteNodeNode) const;
		void loadTorch(const Xml& torchNode) const;
		void loadLightWall(const Xml& wallNode) const;

		void loadHealthComponent(const Xml& entityNode, entt::entity entity) const;
		void loadPosition(const Xml& entityNode, entt::entity entity) const;
		void loadSize(const Xml& entityNode, entt::entity entity) const;
		void loadPositionAndSize(const Xml& entityNode, entt::entity entity) const;

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
		mutable bool mHasLoadedPlayer = false;
	};

}
