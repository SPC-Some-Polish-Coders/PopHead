#include "tiledParser.hpp"

#include "Components/physicsComponents.hpp"
#include "Components/charactersComponents.hpp"
#include "Components/graphicsComponents.hpp"
#include "Components/objectsComponents.hpp"

#include "Scenes/cutSceneManager.hpp"
#include "Scenes/CutScenes/startGameCutscene.hpp"
#include "Scenes/CutScenes/subtitlesBeforeStartGameCutscene.hpp"
#include "Scenes/CutScenes/endingCutscene.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/rect.hpp"
#include "Logs/logs.hpp"
#include "Events/actionEventManager.hpp"

namespace ph {

	TiledParser::TiledParser(CutSceneManager& cutSceneManager, EntitiesTemplateStorage& templatesStorage, entt::registry& gameRegistry, SceneManager& sceneManager)
		:mCutSceneManager(cutSceneManager)
		,mTemplatesStorage(templatesStorage)
		,mGameRegistry(gameRegistry)
		,mHasLoadedPlayer(false)
		,mSceneManager(sceneManager)
	{
	}

	void TiledParser::parseFile(const std::string& filePath) const
	{
		PH_LOG_INFO("Game objects file (" + filePath + ") is being parsed.");

		//mGameData->getAIManager().setAIMode(AIMode::normal);

		Xml mapFile;
		mapFile.loadFromFile(filePath);

		const Xml mapNode = mapFile.getChild("map");
		const Xml gameObjects = findGameObjects(mapNode);

		// TODO: refactor this statement
		if (gameObjects.toString() == "")
			return;

		loadObjects(gameObjects);
		if (filePath.find("arcade") != std::string::npos && filePath.find("arcadeMode") == std::string::npos)
			loadArcadeManager();

		//mGameData->getAIManager().setIsPlayerOnScene(mHasLoadedPlayer);
		ActionEventManager::setEnabled(true);
	}

	bool TiledParser::loadedPlayer() const
	{
		return mHasLoadedPlayer;
	}

	Xml TiledParser::findGameObjects(const Xml& mapNode) const
	{
		const std::vector<Xml> objectGroupNodes = mapNode.getChildren("objectgroup");
		for (const auto& objectGroupNode : objectGroupNodes)
		{
			if ((objectGroupNode.hasAttribute("name") && objectGroupNode.getAttribute("name").toString() == "gameObjects"))
				return objectGroupNode;
		}
		return Xml();
	}

	void TiledParser::loadObjects(const Xml& gameObjectsNode) const
	{
		const std::vector<Xml> objects = gameObjectsNode.getChildren("object");

		for (const auto& gameObjectNode : objects)
		{
			auto objectType = gameObjectNode.getAttribute("type").toString();

			if (objectType == "Zombie") loadZombie(gameObjectNode);
			else if (objectType == "SlowZombie") loadZombie(gameObjectNode, "SlowZombie");
			else if (objectType == "Player") loadPlayer(gameObjectNode);
			else if (objectType == "Camera") loadCamera(gameObjectNode);
			else if (objectType == "Npc") loadNpc(gameObjectNode);
			else if (objectType == "BulletItem") loadBulletItem(gameObjectNode);
			else if (objectType == "Medkit") loadMedkit(gameObjectNode);
			else if (objectType == "Entrance") loadEntrance(gameObjectNode);
			else if (objectType == "SlowDownArea") loadSlowDownArea(gameObjectNode);
			else if (objectType == "ActivateArea") loadActivateArea(gameObjectNode);
			else if (objectType == "CutSceneArea") loadCutSceneArea(gameObjectNode);
			else if (objectType == "Spawner") loadSpawner(gameObjectNode);
			else if (objectType == "LootSpawner") loadLootSpawner(gameObjectNode);
			else if (objectType == "ArcadeSpawner") loadArcadeSpawner(gameObjectNode);
			else if (objectType == "Car") loadCar(gameObjectNode);
			else if (objectType == "Gate") loadGate(gameObjectNode);
			else if (objectType == "Lever") loadLever(gameObjectNode);
			else if (objectType == "CutScene") loadCutScene(gameObjectNode);
			else if (objectType == "CrawlingNpc") loadCrawlingNpc(gameObjectNode);
			else if (objectType == "GateGuardNpc") loadGateGuardNpc(gameObjectNode);
			else if (objectType == "SpriteNode") loadSpriteNode(gameObjectNode);
			else PH_LOG_ERROR("The type of object in map file (" + gameObjectNode.getAttribute("type").toString() + ") is unknown!");
		}
	}

	void TiledParser::loadArcadeManager() const
	{/*
		auto* invisibleObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleObjects->addChild(std::make_unique<ArcadeManager>(mGameData->getGui(), mGameData->getMusicPlayer()));
		mGameData->getAIManager().setAIMode(AIMode::zombieAlwaysLookForPlayer);*/
	}

	void TiledParser::loadZombie(const Xml& zombieNode, std::string zombieTypeName) const
	{
		auto zombie = mTemplatesStorage.createCopy(zombieTypeName, mGameRegistry);
		auto& zombiePosition = mGameRegistry.get<component::BodyRect>(zombie);

		sf::Vector2f position = getPositionAttribute(zombieNode);
		zombiePosition.rect.left = position.x;
		zombiePosition.rect.top = position.y;

		loadHealthComponent(zombieNode, zombie);
	}

	void TiledParser::loadNpc(const Xml& npcNode) const
	{
		auto npc = mTemplatesStorage.createCopy("Npc", mGameRegistry);
		auto& npcPosition = mGameRegistry.get<component::BodyRect>(npc);

		sf::Vector2f position = getPositionAttribute(npcNode);
		npcPosition.rect.left = position.x;
		npcPosition.rect.top = position.y;

		loadHealthComponent(npcNode, npc);
	}

	void TiledParser::loadSpawner(const Xml& spawnerNode) const
	{/*
		auto spawner = std::make_unique<Spawner>(
			mGameData, "spawner",
			Cast::toObjectType(getProperty(spawnerNode, "spawnType").toString()),
			sf::seconds(getProperty(spawnerNode, "spawnFrequency").toFloat()),*/

		auto spawner = mTemplatesStorage.createCopy("Spawner", mGameRegistry);
		loadPosition(spawnerNode, spawner);
	}

	void TiledParser::loadLootSpawner(const Xml& lootSpawnerNode) const
	{/*
		const std::string lootTypeString = getProperty(lootSpawnerNode, "lootType").toString();
		LootType lootType;
		if (lootTypeString == "medkit")
			lootType = LootType::Medkit;
		else if (lootTypeString == "bullets")
			lootType = LootType::Bullets;
		else
			PH_UNEXPECTED_SITUATION("We don't support this loot type");

		auto lootSpawner = std::make_unique<LootSpawner>(
			lootType, mRoot.getChild("LAYER_standingObjects")->getChild("ItemsContainer"), mGameData
			);*/

		auto lootSpawner = mTemplatesStorage.createCopy("lootSpawner", mGameRegistry);
		loadPosition(lootSpawnerNode, lootSpawner);
	}

	void TiledParser::loadArcadeSpawner(const Xml& arcadeSpawnerNode) const
	{/*
		auto arcadeSpawner = std::make_unique<ArcadeSpawner>(
			mGameData, Cast::toObjectType(getProperty(arcadeSpawnerNode, "spawnType").toString()),*/

		auto arcadeSpawner = mTemplatesStorage.createCopy("ArcadeSpawner", mGameRegistry);
		loadPosition(arcadeSpawnerNode, arcadeSpawner);
	}

	void TiledParser::loadEntrance(const Xml& entranceNode) const
	{
		const std::string scenePathRelativeToMapFile = getProperty(entranceNode, "gotoScene").toString();
		const std::string sceneFileName = *getSceneFileName(scenePathRelativeToMapFile);
		const std::string scenePathFromResources = "scenes/" + sceneFileName;

		auto entrance = mTemplatesStorage.createCopy("Entrance", mGameRegistry);
		auto& entranceComponent = mGameRegistry.get<component::Entrance>(entrance);

		entranceComponent.entranceDestination = scenePathRelativeToMapFile;

		loadPosition(entranceNode, entrance);
		loadSize(entranceNode, entrance);

		if (getProperty(entranceNode, "isEntranceWithCustomPosition").toBool())
		{
			sf::Vector2f positionToGo(
				getProperty(entranceNode, "gotoX").toFloat(),
				getProperty(entranceNode, "gotoY").toFloat()
			);

			// TODO: add optional position to component
		}
	}

	void TiledParser::loadSlowDownArea(const Xml& slowDownAreaNode) const
	{
		auto slowDownArea = mTemplatesStorage.createCopy("SlowDownArea", mGameRegistry);
		loadPosition(slowDownAreaNode, slowDownArea);
		loadSize(slowDownAreaNode, slowDownArea);
	}

	void TiledParser::loadActivateArea(const Xml& activateAreaNode) const
	{/*
		const std::string areaName = getProperty(activateAreaNode, "areaName").toString();*/

		auto activateArea = mTemplatesStorage.createCopy("ActivateArea", mGameRegistry);
		loadPosition(activateAreaNode, activateArea);
		loadSize(activateAreaNode, activateArea);
	}

	void TiledParser::loadCutSceneArea(const Xml& cutSceneAreaNode) const
	{/*
		const std::string cutSceneName = getProperty(cutSceneAreaNode, "cutSceneName").toString();*/

		auto cutSceneArea = mTemplatesStorage.createCopy("CutSceneArea", mGameRegistry);
		loadPosition(cutSceneAreaNode, cutSceneArea);
		loadSize(cutSceneAreaNode, cutSceneArea);
	}

	std::optional<std::string> TiledParser::getSceneFileName(const std::string& scenePathRelativeToMapFile) const
	{
		std::size_t beginOfFileName = scenePathRelativeToMapFile.find_last_of('/');
		if (beginOfFileName == std::string::npos)
			return std::nullopt;
		return scenePathRelativeToMapFile.substr(beginOfFileName, scenePathRelativeToMapFile.size());
	}

	void TiledParser::loadGate(const Xml& gateNode) const
	{
		auto gate = mTemplatesStorage.createCopy("Gate", mGameRegistry);
		loadPosition(gateNode, gate);
		//loadSize(gateNode, gate);
	}

	void TiledParser::loadLever(const Xml& leverNode) const
	{
		auto lever = mTemplatesStorage.createCopy("Lever", mGameRegistry);
		loadPosition(leverNode, lever);
	}

	void TiledParser::loadCar(const Xml& carNode) const
	{/*
		auto car = std::make_unique<Car>(
			getProperty(carNode, "acceleration").toFloat(),
			getProperty(carNode, "slowingDown").toFloat(),
			sf::Vector2f(getProperty(carNode, "directionX").toFloat(), getProperty(carNode, "directionY").toFloat())
			);*/

		auto car = mTemplatesStorage.createCopy("Car", mGameRegistry);
		loadPosition(carNode, car);
		loadSize(carNode, car);
	}

	void TiledParser::loadCamera(const Xml& cameraNode) const
	{
		auto camera = mTemplatesStorage.createCopy("Camera", mGameRegistry);
		const FloatRect cameraBounds(getPositionAttribute(cameraNode), getSizeAttribute(cameraNode));

		auto& cameraComponent = mGameRegistry.get<component::Camera>(camera);
		cameraComponent.camera = Camera(cameraBounds);
	}

	void TiledParser::loadPlayer(const Xml& playerNode) const
	{
		auto player = mTemplatesStorage.createCopy("Player", mGameRegistry);
		auto& playerPosition = mGameRegistry.get<component::BodyRect>(player);
		
		sf::Vector2f position;

		if (mSceneManager.hasPlayerPosition())
			position = mSceneManager.getPlayerPosition();
		else
			position = getPositionAttribute(playerNode);

		playerPosition.rect.left = position.x;
		playerPosition.rect.top = position.y;

		mHasLoadedPlayer = true;
	}

	void TiledParser::loadCutScene(const Xml& cutSceneNode) const
	{/*
		if (!getProperty(cutSceneNode, "isStartingCutSceneOnThisMap").toBool())
			return;

		const std::string name = getProperty(cutSceneNode, "cutSceneName").toString();

		if (name == "subtitlesBeforeStartGameCutscene") {
			auto subtitlesBeforeStartGameCutscene = std::make_unique<SubtitlesBeforeStartGameCutscene>(
				mRoot,
				mGameData->getSceneManager(),
				mGameData->getGui()
				);
			mCutSceneManager.activateCutscene(std::move(subtitlesBeforeStartGameCutscene));
		}
		else if (name == "startGameCutScene") {
			auto startGameCutScene = std::make_unique<StartGameCutScene>(
				mRoot,
				mGameData->getRenderer().getCamera(),
				mGameData->getSoundPlayer(),
				mGameData->getMusicPlayer(),
				mGameData->getGui(),
				mGameData
				);
			mCutSceneManager.activateCutscene(std::move(startGameCutScene));
		}*/
	}

	void TiledParser::loadCrawlingNpc(const Xml& crawlingNpcNode) const
	{/*
		if (getProperty(crawlingNpcNode, "isAlreadyDead").toBool())
			crawlingNpc->die();*/

		auto crawlingNpc = mTemplatesStorage.createCopy("CrawlingNpc", mGameRegistry);
		loadPosition(crawlingNpcNode, crawlingNpc);
	}

	void TiledParser::loadGateGuardNpc(const Xml& gateGuardNpcNode) const
	{
		auto gateGuard = mTemplatesStorage.createCopy("GateGuardNpc", mGameRegistry);
		loadPosition(gateGuardNpcNode, gateGuard);
	}

	void TiledParser::loadBulletItem(const Xml& bulletItemNode) const
	{
		auto bulletItem = mTemplatesStorage.createCopy("BulletItem", mGameRegistry);
		loadPosition(bulletItemNode, bulletItem);
	}

	void TiledParser::loadMedkit(const Xml& medkitItemNode) const
	{
		auto medkit = mTemplatesStorage.createCopy("Medkit", mGameRegistry);
		loadPosition(medkitItemNode, medkit);
	}

	void TiledParser::loadSpriteNode(const Xml& spriteNodeNode) const
	{
		/*const std::string texturePath = getProperty(spriteNodeNode, "texturePath").toString();
		auto spriteNode = std::make_unique<SpriteNode>(mGameData->getTextures().get(texturePath));
		spriteNode->setPosition(getPositionAttribute(spriteNodeNode));
		auto* standingObjects = mRoot.getChild("LAYER_standingObjects");
		standingObjects->addChild(std::move(spriteNode));*/
	}

	void TiledParser::loadHealthComponent(const Xml& entityNode, entt::entity entity) const
	{
		auto& healthComponent = mGameRegistry.get<component::Health>(entity);
		healthComponent.healthPoints = getProperty(entityNode, "hp").toInt();
		healthComponent.maxHealthPoints = getProperty(entityNode, "maxHp").toInt();
	}

	void TiledParser::loadPosition(const Xml& entityNode, entt::entity entity) const
	{
		auto& bodyRect = mGameRegistry.get<component::BodyRect>(entity);
		auto position = getPositionAttribute(entityNode);
		bodyRect.rect.left = position.x;
		bodyRect.rect.top = position.y;
	}

	void TiledParser::loadSize(const Xml& entityNode, entt::entity entity) const
	{
		auto& bodyRect = mGameRegistry.get<component::BodyRect>(entity);
		auto size = getSizeAttribute(entityNode);
		bodyRect.rect.width = size.x;
		bodyRect.rect.height = size.y;
	}

	Xml TiledParser::getProperty(const Xml& objectNode, const std::string& propertyName) const
	{
		if (hasCustomProperty(objectNode, propertyName))
			return getCustomProperties(objectNode, propertyName);
		else
			return getDefaultProperties(objectNode.getAttribute("type").toString(), propertyName);
	}

	bool TiledParser::hasCustomProperty(const Xml& gameObjectNode, const std::string& propertyName) const
	{
		if (gameObjectNode.getChildren("properties").size() != 0)
		{
			auto properties = gameObjectNode.getChild("properties").getChildren("property");
			for (const auto& property : properties)
			{
				if (property.getAttribute("name").toString() == propertyName)
					return true;
			}
			return false;
		}
		return false;
	}

	Xml TiledParser::getCustomProperties(const Xml& gameObjectNode, const std::string& name) const
	{
		const Xml propertiesNode = gameObjectNode.getChild("properties");
		auto properties = propertiesNode.getChildren("property");
		for (const auto& property : properties)
			if (property.getAttribute("name").toString() == name)
				return property.getAttribute("value");

		return Xml();
	}

	Xml TiledParser::getDefaultProperties(const std::string& objectName, const std::string& propertyName) const
	{
		const std::vector<Xml> objectNodes = getObjectTypeNodes();
		for (const auto& objectNode : objectNodes)
		{
			if (objectNode.getAttribute("name").toString() == objectName)
			{
				std::vector<Xml> propertiesNodes = objectNode.getChildren("property");
				for (const auto& propertyNode : propertiesNodes)
				{
					if (propertyNode.getAttribute("name").toString() == propertyName)
						return propertyNode.getAttribute("default");
				}
			}
		}
		return Xml();
	}

	std::vector<Xml> TiledParser::getObjectTypeNodes() const
	{
		Xml objectTypesFile;
		objectTypesFile.loadFromFile("scenes/map/objecttypes.xml");
		const Xml objectTypesNode = objectTypesFile.getChild("objecttypes");
		std::vector<Xml> getObjectTypeNodes = objectTypesNode.getChildren("objecttype");
		return getObjectTypeNodes;
	}

	sf::Vector2f TiledParser::getPositionAttribute(const Xml& DrawableGameObjectNode) const
	{
		return sf::Vector2f(
			DrawableGameObjectNode.getAttribute("x").toFloat(),
			DrawableGameObjectNode.getAttribute("y").toFloat()
		);
	}

	sf::Vector2f TiledParser::getSizeAttribute(const Xml& DrawableGameObjectNode) const
	{
		return sf::Vector2f(
			DrawableGameObjectNode.getAttribute("width").toFloat(),
			DrawableGameObjectNode.getAttribute("height").toFloat()
		);
	}

}
