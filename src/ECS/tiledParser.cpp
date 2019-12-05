#include "tiledParser.hpp"

#include "Components/physicsComponents.hpp"
#include "Components/charactersComponents.hpp"
#include "Components/graphicsComponents.hpp"

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
		: mCutSceneManager(cutSceneManager)
		, mTemplatesStorage(templatesStorage)
		, mGameRegistry(gameRegistry)
		, mHasLoadedPlayer(false)
		, mSceneManager(sceneManager)
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
		if (gameObjects.toString() == "")
			return;

		loadObjects(gameObjects);
		if (filePath.find("arcade") != std::string::npos && filePath.find("arcadeMode") == std::string::npos)
			loadArcadeManager();

		//mGameData->getAIManager().setIsPlayerOnScene(mHasLoadedPlayer);
		ActionEventManager::setEnabled(true);
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

		loadLayerObjects();
		loadContainerObjects();

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

		/*if (mHasLoadedPlayer)
		{
			const auto& player = *mRoot.getChild("LAYER_standingObjects")->getChild("player");
			auto playerPosition = player.getPosition();
			mGameData->getRenderer().getCamera().setCenter(playerPosition);
		}*/
	}

	void TiledParser::loadArcadeManager() const
	{/*
		auto* invisibleObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleObjects->addChild(std::make_unique<ArcadeManager>(mGameData->getGui(), mGameData->getMusicPlayer()));
		mGameData->getAIManager().setAIMode(AIMode::zombieAlwaysLookForPlayer);*/
	}

	void TiledParser::loadLayerObjects() const
	{/*
		mRoot.addChild(std::make_unique<GameObject>("LAYER_invisibleObjects"));
		mRoot.addChild(std::make_unique<LyingGameObjectsLayer>());
		mRoot.addChild(std::make_unique<StandingGameObjectsLayer>(mGameData->getPhysicsEngine()));*/
	}

	void TiledParser::loadContainerObjects() const
	{/*
		auto* standingObjects = mRoot.getChild("LAYER_standingObjects");
		standingObjects->addChild(std::make_unique<ParticlesSystem>(mGameData->getRenderer()));
		standingObjects->addChild(std::make_unique<ItemsContainer>());*/
	}

	/*bool TiledParser::isObjectOfType(const Xml& gameObjectNode, const std::string& typeName) const
	{
		return gameObjectNode.getAttribute("type").toString() == typeName;
	}*/

	void TiledParser::loadZombie(const Xml& zombieNode, std::string zombieTypeName) const
	{
		auto zombie = mTemplatesStorage.createCopy(zombieTypeName, mGameRegistry);
		auto& zombiePosition = mGameRegistry.get<component::BodyRect>(zombie);

		sf::Vector2f position = getPositionAttribute(zombieNode);
		zombiePosition.rect.left = position.x;
		zombiePosition.rect.top = position.y;

		loadHealthComponent(zombieNode, zombie);

		mHasLoadedPlayer = true;
	}

	void TiledParser::loadNpc(const Xml& npcNode) const
	{/*
		auto npc = std::make_unique<Npc>(mGameData);

		npc->setPosition(getPositionAttribute(npcNode));
		npc->setHp(getProperty(npcNode, "hp").toInt());
		npc->setMaxHp(getProperty(npcNode, "maxHp").toUnsigned());
		const std::string texturePath = "textures/characters/" + getProperty(npcNode, "textureFileName").toString();
		npc->getSprite().setTexture(mGameData->getTextures().get(texturePath));

		auto* standingObjects = mRoot.getChild("LAYER_standingObjects");
		standingObjects->addChild(std::move(npc));*/
	}

	void TiledParser::loadSpawner(const Xml& spawnerNode) const
	{/*
		auto spawner = std::make_unique<Spawner>(
			mGameData, "spawner",
			Cast::toObjectType(getProperty(spawnerNode, "spawnType").toString()),
			sf::seconds(getProperty(spawnerNode, "spawnFrequency").toFloat()),
			getPositionAttribute(spawnerNode)
			);

		auto* invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleGameObjects->addChild(std::move(spawner));*/
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
			);
		lootSpawner->setPosition(getPositionAttribute(lootSpawnerNode));
		auto* invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleGameObjects->addChild(std::move(lootSpawner));*/
	}

	void TiledParser::loadArcadeSpawner(const Xml& arcadeSpawnerNode) const
	{/*
		auto arcadeSpawner = std::make_unique<ArcadeSpawner>(
			mGameData, Cast::toObjectType(getProperty(arcadeSpawnerNode, "spawnType").toString()),
			getPositionAttribute(arcadeSpawnerNode));

		auto* invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleGameObjects->addChild(std::move(arcadeSpawner));*/
	}

	void TiledParser::loadEntrance(const Xml& entranceNode) const
	{/*
		const std::string scenePathRelativeToMapFile = getProperty(entranceNode, "gotoScene").toString();
		const std::string sceneFileName = *getSceneFileName(scenePathRelativeToMapFile);
		const std::string scenePathFromResources = "scenes/" + sceneFileName;

		std::unique_ptr<Entrance> entrance;

		if (getProperty(entranceNode, "isEntranceWithCustomPosition").toBool())
		{
			sf::Vector2f positionToGo(
				getProperty(entranceNode, "gotoX").toFloat(),
				getProperty(entranceNode, "gotoY").toFloat()
			);

			entrance = std::make_unique<Entrance>(
				mGameData->getSceneManager(),
				scenePathFromResources,
				"entrance",
				getSizeAttribute(entranceNode),
				getPositionAttribute(entranceNode),
				positionToGo
				);
		}
		else
		{
			entrance = std::make_unique<Entrance>(
				mGameData->getSceneManager(),
				scenePathFromResources,
				"entrance",
				getSizeAttribute(entranceNode),
				getPositionAttribute(entranceNode)
				);
		}

		auto* invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleGameObjects->addChild(std::move(entrance));*/
	}

	void TiledParser::loadSlowDownArea(const Xml& slowDownAreaNode) const
	{/*
		const sf::Vector2f position = getPositionAttribute(slowDownAreaNode);
		const sf::Vector2f size = getSizeAttribute(slowDownAreaNode);
		const sf::FloatRect area(position.x, position.y, size.x, size.y);
		auto slowDownArea = std::make_unique<SlowDownArea>(area);
		auto* invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleGameObjects->addChild(std::move(slowDownArea));*/
	}

	void TiledParser::loadActivateArea(const Xml& activateAreaNode) const
	{/*
		const sf::Vector2f position = getPositionAttribute(activateAreaNode);
		const sf::Vector2f size = getSizeAttribute(activateAreaNode);
		const sf::FloatRect area(position.x, position.y, size.x, size.y);
		const std::string areaName = getProperty(activateAreaNode, "areaName").toString();
		auto activateArea = std::make_unique<ActivateArea>(areaName, area);
		auto* invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleGameObjects->addChild(std::move(activateArea));*/
	}

	void TiledParser::loadCutSceneArea(const Xml& cutSceneAreaNode) const
	{/*
		const sf::Vector2f position = getPositionAttribute(cutSceneAreaNode);
		const sf::Vector2f size = getSizeAttribute(cutSceneAreaNode);
		const sf::FloatRect area(position.x, position.y, size.x, size.y);
		const std::string cutSceneName = getProperty(cutSceneAreaNode, "cutSceneName").toString();
		auto cutSceneArea = std::make_unique<CutSceneArea>(mGameData, cutSceneName, area);
		auto* invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
		invisibleGameObjects->addChild(std::move(cutSceneArea));*/
	}

	std::optional<std::string> TiledParser::getSceneFileName(const std::string& scenePathRelativeToMapFile) const
	{/*
		std::size_t beginOfFileName = scenePathRelativeToMapFile.find_last_of('/');
		if (beginOfFileName == std::string::npos)
			return std::nullopt;
		return scenePathRelativeToMapFile.substr(beginOfFileName, scenePathRelativeToMapFile.size());*/
		return std::nullopt; //TEMPORARY LINE
	}

	void TiledParser::loadGate(const Xml& gateNode) const
	{/*
		auto& texture = mGameData->getTextures().get("textures/others/gate.png");
		auto gate = std::make_unique<Gate>(texture, getPositionAttribute(gateNode), mGameData->getPhysicsEngine(), false);
		auto* lyingObjects = mRoot.getChild("LAYER_lyingObjects");
		lyingObjects->addChild(std::move(gate));*/
	}

	void TiledParser::loadLever(const Xml& leverNode) const
	{/*
		auto& leverTexture = mGameData->getTextures().get("textures/others/lever.png");
		auto& hintTexture = mGameData->getTextures().get("textures/others/pressSpaceHint.png");
		auto lever = std::make_unique<Lever>(leverTexture, hintTexture);
		lever->setPosition(getPositionAttribute(leverNode));
		auto* lyingObjects = mRoot.getChild("LAYER_lyingObjects");
		lyingObjects->addChild(std::move(lever));*/
	}

	void TiledParser::loadCar(const Xml& carNode) const
	{/*
		auto& texture = mGameData->getTextures().get("textures/vehicles/car.png");
		auto car = std::make_unique<Car>(
			getProperty(carNode, "acceleration").toFloat(),
			getProperty(carNode, "slowingDown").toFloat(),
			sf::Vector2f(getProperty(carNode, "directionX").toFloat(), getProperty(carNode, "directionY").toFloat()),
			texture
			);
		auto position = getPositionAttribute(carNode);
		car->setPosition(position);

		sf::FloatRect carRect(position, sf::Vector2f(texture.getSize()));
		mGameData->getPhysicsEngine().createStaticBodyAndGetTheReference(carRect);

		auto* lyingObjects = mRoot.getChild("LAYER_lyingObjects");
		lyingObjects->addChild(std::move(car));*/
	}

	void TiledParser::loadCamera(const Xml& cameraNode) const
	{/*
		const sf::Vector2f cameraTopLeftCornerPosition = getPositionAttribute(cameraNode);
		const sf::Vector2f cameraViewSize = getSizeAttribute(cameraNode);
		const FloatRect cameraBounds(
			cameraTopLeftCornerPosition.x, cameraTopLeftCornerPosition.y,
			cameraViewSize.x, cameraViewSize.y
		);
		const sf::Vector2f cameraCenter = cameraBounds.getCenter();

		auto& camera = mGameData->getRenderer().getCamera();
		camera.setSize(cameraViewSize);
		camera.setCenter(cameraCenter);*/
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
		auto crawlingNpc = std::make_unique<CrawlingNpc>(mGameData);
		crawlingNpc->setPosition(getPositionAttribute(crawlingNpcNode));
		if (getProperty(crawlingNpcNode, "isAlreadyDead").toBool())
			crawlingNpc->die();
		auto* lyingObjects = mRoot.getChild("LAYER_lyingObjects");
		lyingObjects->addChild(std::move(crawlingNpc));*/
	}

	void TiledParser::loadGateGuardNpc(const Xml& gateGuardNpcNode) const
	{/*
		auto gateGuardNpc = std::make_unique<GateGuard>(mGameData);
		gateGuardNpc->setPosition(getPositionAttribute(gateGuardNpcNode));
		mRoot.addChild(std::move(gateGuardNpc));*/
	}

	void TiledParser::loadBulletItem(const Xml& bulletItemNode) const
	{/*
		auto bulletItem = std::make_unique<BulletItem>(mGameData);
		bulletItem->setPosition(getPositionAttribute(bulletItemNode));
		auto* standingObjects = mRoot.getChild("LAYER_standingObjects");
		standingObjects->getChild("ItemsContainer")->addChild(std::move(bulletItem));*/
	}

	void TiledParser::loadMedkit(const Xml& bulletItemNode) const
	{/*
		auto medkitItem = std::make_unique<Medkit>(mGameData);
		medkitItem->setPosition(getPositionAttribute(bulletItemNode));
		auto* standingObjects = mRoot.getChild("LAYER_standingObjects");
		standingObjects->getChild("ItemsContainer")->addChild(std::move(medkitItem));*/
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
