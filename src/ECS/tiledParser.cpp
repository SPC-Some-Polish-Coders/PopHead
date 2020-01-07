#include "tiledParser.hpp"

#include "Components/physicsComponents.hpp"
#include "Components/charactersComponents.hpp"
#include "Components/graphicsComponents.hpp"
#include "Components/objectsComponents.hpp"
#include "Components/itemComponents.hpp"
#include "Components/particleComponents.hpp"

#include "Scenes/cutSceneManager.hpp"
#include "Scenes/CutScenes/startGameCutscene.hpp"
#include "Scenes/CutScenes/subtitlesBeforeStartGameCutscene.hpp"
#include "Scenes/CutScenes/endingCutscene.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "Events/actionEventManager.hpp"
#include "Renderer/API/shader.hpp"

namespace ph {

	TiledParser::TiledParser(CutSceneManager& cutSceneManager, EntitiesTemplateStorage& templatesStorage, entt::registry& gameRegistry,
	                         SceneManager& sceneManager, TextureHolder& textureHolder)
		:mCutSceneManager(cutSceneManager)
		,mTemplatesStorage(templatesStorage)
		,mGameRegistry(gameRegistry)
		,mSceneManager(sceneManager)
		,mTextureHolder(textureHolder)
	{
	}

	void TiledParser::parseFile(const std::string& filePath) const
	{
		PH_LOG_INFO("Game objects file (" + filePath + ") is being parsed.");

		Xml mapFile;
		mapFile.loadFromFile(filePath);

		const Xml gameObjects = findGameObjects(*mapFile.getChild("map"));

		// TODO: refactor this statement
		if (gameObjects.toString() == "")
			return;

		loadObjects(gameObjects);

		ActionEventManager::setEnabled(true);
	}

	Xml TiledParser::findGameObjects(const Xml& mapNode) const
	{
		const std::vector<Xml> objectGroupNodes = mapNode.getChildren("objectgroup");
		for (const auto& objectGroupNode : objectGroupNodes)
			if ((objectGroupNode.getAttribute("name")->toString() == "gameObjects"))
				return objectGroupNode;
		return Xml();
	}

	void TiledParser::loadObjects(const Xml& gameObjectsNode) const
	{
		const std::vector<Xml> objects = gameObjectsNode.getChildren("object");

		for (const auto& gameObjectNode : objects)
		{
			auto objectType = gameObjectNode.getAttribute("type")->toString();

			if (objectType == "Zombie") loadZombie(gameObjectNode);
			else if (objectType == "SlowZombie") loadZombie(gameObjectNode, "SlowZombie");
			else if (objectType == "Player") loadPlayer(gameObjectNode);
			else if (objectType == "Camera") loadCamera(gameObjectNode);
			else if (objectType == "BulletBox") loadBulletBox(gameObjectNode);
			else if (objectType == "Medkit") loadMedkit(gameObjectNode);
			else if (objectType == "Entrance") loadEntrance(gameObjectNode);
			else if (objectType == "VelocityChangingArea") loadVelocityChangingArea(gameObjectNode);
			else if (objectType == "PushingArea") loadPushingArea(gameObjectNode);
			else if (objectType == "HintArea") loadHintArea(gameObjectNode);
			else if (objectType == "LootSpawner") loadLootSpawner(gameObjectNode);
			else if (objectType == "ArcadeSpawner") loadArcadeSpawner(gameObjectNode);
			else if (objectType == "Car") loadCar(gameObjectNode);
			else if (objectType == "Gate") loadGate(gameObjectNode);
			else if (objectType == "Lever") loadLever(gameObjectNode);
			else if (objectType == "CutScene") loadCutScene(gameObjectNode);
			else if (objectType == "CrawlingNpc") loadCrawlingNpc(gameObjectNode);
			else if (objectType == "GateGuardNpc") loadGateGuardNpc(gameObjectNode);
			else if (objectType == "Sprite") loadSprite(gameObjectNode);
			else if (objectType == "Torch") loadTorch(gameObjectNode);
			else if (objectType == "LightWall") loadLightWall(gameObjectNode);
			else if (objectType == "FlowingRiver") loadFlowingRiver(gameObjectNode);
			else PH_LOG_ERROR("The type of object in map file (" + gameObjectNode.getAttribute("type")->toString() + ") is unknown!");
		}
	}

	void TiledParser::loadZombie(const Xml& zombieNode, std::string zombieTypeName) const
	{
		auto zombie = mTemplatesStorage.createCopy(zombieTypeName, mGameRegistry);
		loadPosition(zombieNode, zombie);
		loadHealthComponent(zombieNode, zombie);
	}

	void TiledParser::loadLootSpawner(const Xml& lootSpawnerNode) const
	{
		auto lootSpawnerEntity = mTemplatesStorage.createCopy("LootSpawner", mGameRegistry);
		loadPosition(lootSpawnerNode, lootSpawnerEntity);
		const std::string lootTypeString = getProperty(lootSpawnerNode, "lootType").toString();
		auto& lootSpawner = mGameRegistry.get<component::LootSpawner>(lootSpawnerEntity);
		if (lootTypeString == "medkit")
			lootSpawner.type= component::LootSpawner::Medkit;
		else if (lootTypeString == "bullets")
			lootSpawner.type = component::LootSpawner::Bullets;
		else
			PH_UNEXPECTED_SITUATION("We don't support this loot type");
	}

	void TiledParser::loadArcadeSpawner(const Xml& arcadeSpawnerNode) const
	{
		auto arcadeSpawner = mTemplatesStorage.createCopy("ArcadeSpawner", mGameRegistry);
		loadPosition(arcadeSpawnerNode, arcadeSpawner);
		loadSize(arcadeSpawnerNode, arcadeSpawner);

		auto& waves = mGameRegistry.get<component::ArcadeSpawner>(arcadeSpawner).waves;
		waves[0].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave01-normalZombies").toUnsigned();
		waves[0].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave01-slowZombies").toUnsigned();
		waves[1].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave02-normalZombies").toUnsigned();
		waves[1].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave02-slowZombies").toUnsigned();
		waves[2].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave03-normalZombies").toUnsigned();
		waves[2].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave03-slowZombies").toUnsigned();
		waves[3].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave04-normalZombies").toUnsigned();
		waves[3].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave04-slowZombies").toUnsigned();
		waves[4].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave05-normalZombies").toUnsigned();
		waves[4].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave05-slowZombies").toUnsigned();
		waves[5].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave06-normalZombies").toUnsigned();
		waves[5].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave06-slowZombies").toUnsigned();
		waves[6].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave07-normalZombies").toUnsigned();
		waves[6].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave07-slowZombies").toUnsigned();
		waves[7].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave08-normalZombies").toUnsigned();
		waves[7].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave08-slowZombies").toUnsigned();
		waves[8].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave09-normalZombies").toUnsigned();
		waves[8].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave09-slowZombies").toUnsigned();
		waves[9].normalZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave10-normalZombies").toUnsigned();
		waves[9].slowZombiesToSpawn = getProperty(arcadeSpawnerNode, "wave10-slowZombies").toUnsigned();
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

		if (getProperty(entranceNode, "isEntranceWithCustomPosition").toBool()) {
			entranceComponent.playerSpawnPosition = sf::Vector2f(
				getProperty(entranceNode, "gotoX").toFloat(),
				getProperty(entranceNode, "gotoY").toFloat()
			);
		}
	}

	void TiledParser::loadVelocityChangingArea(const Xml& velocityChanginAreaNode) const
	{
		auto entity = mTemplatesStorage.createCopy("VelocityChangingArea", mGameRegistry);
		loadPosition(velocityChanginAreaNode, entity);
		loadSize(velocityChanginAreaNode, entity);
		float& areaSpeedMultiplier = mGameRegistry.get<component::AreaVelocityChangingEffect>(entity).areaSpeedMultiplier;
		areaSpeedMultiplier = getProperty(velocityChanginAreaNode, "velocityMultiplier").toFloat();
	}

	void TiledParser::loadPushingArea(const Xml& pushingAreaNode) const
	{
		auto entity = mTemplatesStorage.createCopy("PushingArea", mGameRegistry);
		loadPosition(pushingAreaNode, entity);
		loadSize(pushingAreaNode, entity);
		auto& pushDirection = mGameRegistry.get<component::PushingArea>(entity);
		pushDirection.pushForce.x = getProperty(pushingAreaNode, "pushForceX").toFloat();
		pushDirection.pushForce.y = getProperty(pushingAreaNode, "pushForceY").toFloat();
	}

	void TiledParser::loadHintArea(const Xml& hintAreaNode) const
	{
		auto entity = mTemplatesStorage.createCopy("HintArea", mGameRegistry);
		loadPosition(hintAreaNode, entity);
		loadSize(hintAreaNode, entity);
		auto& hintDetails = mGameRegistry.get<component::Hint>(entity);
		hintDetails.hintName = getProperty(hintAreaNode, "hintName").toString();
	}

	void TiledParser::loadCutScene(const Xml& cutSceneNode) const
	{
		const std::string cutSceneName = getProperty(cutSceneNode, "cutSceneName").toString();
		auto cutSceneEntity = mTemplatesStorage.createCopy("CutScene", mGameRegistry);
		loadPositionAndOptionalSize(cutSceneNode, cutSceneEntity);
		auto& cutscene = mGameRegistry.get<component::CutScene>(cutSceneEntity);
		cutscene.name = getProperty(cutSceneNode, "name").toString();
		cutscene.isStartingCutSceneOnThisMap = getProperty(cutSceneNode, "isStartingCutSceneOnThisMap").toBool();
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
	{
		auto entityCar = mTemplatesStorage.createCopy("Car", mGameRegistry);
		loadPosition(carNode, entityCar);
		auto& car = mGameRegistry.get<component::Car>(entityCar);
		car.acceleration = getProperty(carNode, "acceleration").toFloat();
		car.slowingDown = getProperty(carNode, "slowingDown").toFloat();
		car.velocity = getProperty(carNode, "velocity").toFloat();
		car.direction.x = getProperty(carNode, "directionX").toFloat();
		car.direction.y = getProperty(carNode, "directionY").toFloat();
		car.shouldSlowDown = getProperty(carNode, "shouldSlowDown").toBool();
		car.shouldSpeedUp = getProperty(carNode, "shouldSpeedUp").toBool();
		car.velocity = getProperty(carNode, "velocity").toFloat();
	}

	void TiledParser::loadCamera(const Xml& cameraNode) const
	{
		if(getProperty(cameraNode, "isValid").toBool()) {
			auto cameraEntity = mTemplatesStorage.createCopy("Camera", mGameRegistry);
			auto& camera = mGameRegistry.get<component::Camera>(cameraEntity);
			const sf::Vector2f pos = getPositionAttribute(cameraNode);
			const sf::Vector2f size = getSizeAttribute(cameraNode);
			const sf::Vector2f center(pos + (size / 2.f));
			camera.camera = Camera(center, size);
			camera.name = getProperty(cameraNode, "name").toString();
		}
	}

	void TiledParser::loadPlayer(const Xml& playerNode) const
	{
		mHasLoadedPlayer = true;

		auto player = mTemplatesStorage.createCopy("Player", mGameRegistry);
		auto& playerBody = mGameRegistry.get<component::BodyRect>(player);
		auto& playerCamera = mGameRegistry.get<component::Camera>(player);
		
		playerBody.rect.setPosition(mSceneManager.hasPlayerPositionForNextScene() ?
			mSceneManager.getPlayerPositionForNextScene() : getPositionAttribute(playerNode));

		playerCamera.camera = Camera(playerBody.rect.getCenter(), sf::Vector2f(640, 360));

		mTemplatesStorage.createCopy("Pistol", mGameRegistry);
		auto shotgun = mTemplatesStorage.createCopy("Shotgun", mGameRegistry);
		auto melee = mTemplatesStorage.createCopy("BaseballBat", mGameRegistry);
		mGameRegistry.assign<component::CurrentMeleeWeapon>(melee);

		if(getProperty(playerNode, "hasFlashlight").toBool()) {
			component::LightSource flashlight;
			flashlight.offset = {10.f, 10.f};
			flashlight.color = sf::Color(255, 255, 255, 90);
			flashlight.startAngle = 40.f;
			flashlight.endAngle = 40.f;
			flashlight.attenuationAddition = 0.1f;
			flashlight.attenuationFactor = 1.f;
			flashlight.attenuationSquareFactor = 1.5f;
			mGameRegistry.assign_or_replace<component::LightSource>(player, flashlight);
		}	
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

	void TiledParser::loadBulletBox(const Xml& bulletItemNode) const
	{
		auto bulletBoxEntity = mTemplatesStorage.createCopy("BulletBox", mGameRegistry);
		loadPosition(bulletItemNode, bulletBoxEntity);
		auto& bullets= mGameRegistry.get<component::Bullets>(bulletBoxEntity);
		bullets.numOfPistolBullets = getProperty(bulletItemNode, "numOfPistolBullets").toInt();
		bullets.numOfShotgunBullets = getProperty(bulletItemNode, "numOfShotgunBullets").toInt();
	}

	void TiledParser::loadMedkit(const Xml& medkitItemNode) const
	{
		auto medkit = mTemplatesStorage.createCopy("Medkit", mGameRegistry);
		loadPosition(medkitItemNode, medkit);
	}

	void TiledParser::loadSprite(const Xml& spriteNode) const
	{
		// create sprite entity
		auto spriteEntity = mTemplatesStorage.createCopy("Sprite", mGameRegistry);
		auto& [rq, body] = mGameRegistry.get<component::RenderQuad, component::BodyRect>(spriteEntity);

		// load texture
		const std::string texturePath = getProperty(spriteNode, "texturePath").toString();
		if(texturePath != "none") {
			if(mTextureHolder.load(texturePath))
				rq.texture = &mTextureHolder.get(texturePath);
			else
				PH_EXIT_GAME("TiledParser::loadSprite() wasn't able to load texture \"" + texturePath + "\"");
		}

		// load texture rect
		if(getProperty(spriteNode, "activeTextureRect").toBool()) {
			mGameRegistry.assign_or_replace<component::TextureRect>(
				spriteEntity,
				IntRect(
					getProperty(spriteNode, "textureRectLeft").toInt(),
					getProperty(spriteNode, "textureRectTop").toInt(),
					getProperty(spriteNode, "textureRectWidth").toInt(),
					getProperty(spriteNode, "textureRectHeight").toInt()
				)
			);
		}

		// load hidden for renderer
		if(getProperty(spriteNode, "hiddenForRenderer").toBool())
			mGameRegistry.assign_or_replace<component::HiddenForRenderer>(spriteEntity);

		// load shader
		const std::string shaderName = getProperty(spriteNode, "shaderName").toString();
		if(shaderName != "none") {
			const std::string vertexShaderFilepath = getProperty(spriteNode, "vertexShaderFilepath").toString();
			PH_ASSERT_CRITICAL(vertexShaderFilepath != "none", "TiledParser::loadSprite(): Sprite has 'shaderName' but doesn't have 'vertexShaderFilepath'!");
			const std::string fragmentShaderFilepath = getProperty(spriteNode, "vertexShaderFilepath").toString();
			PH_ASSERT_CRITICAL(fragmentShaderFilepath != "none", "TiledParser::loadSprite(): Sprite has 'shaderName' but doesn't have 'fragmentShaderFilepath'!");

			auto& sl = ShaderLibrary::getInstance();
			if(sl.loadFromFile(shaderName, vertexShaderFilepath.c_str(), fragmentShaderFilepath.c_str()))
				rq.shader = sl.get(shaderName);
			else
				PH_EXIT_GAME("EntitiesParser::parseRenderQuad() wasn't able to load shader!");
		}
		else
			rq.shader = nullptr;

		// load rotation and rotation origin
		rq.rotation = getProperty(spriteNode, "rotation").toFloat();
		rq.rotationOrigin.x = getProperty(spriteNode, "rotationOriginX").toFloat();
		rq.rotationOrigin.y = getProperty(spriteNode, "rotationOriginY").toFloat();

		// load z
		rq.z = getProperty(spriteNode, "z").toUnsignedChar();

		// TODO: Load color
		rq.color = sf::Color::White;

		// load body rect
		loadPositionAndSize(spriteNode, spriteEntity);
	}

	void TiledParser::loadTorch(const Xml& torchNode) const
	{
		auto entity = mTemplatesStorage.createCopy("Torch", mGameRegistry);
		loadPosition(torchNode, entity);
	}

	void TiledParser::loadLightWall(const Xml& wallNode) const
	{
		auto entity = mTemplatesStorage.createCopy("LightWall", mGameRegistry);
		loadPositionAndSize(wallNode, entity);
	}

	void TiledParser::loadFlowingRiver(const Xml& flowingRiverNode) const
	{
		auto entity = mTemplatesStorage.createCopy("FlowingRiver", mGameRegistry);
		auto& [pushingArea, particleEmitter, body] = mGameRegistry.get<component::PushingArea, component::ParticleEmitter, component::BodyRect>(entity);
		body.rect.setPosition(getPositionAttribute(flowingRiverNode));
		const sf::Vector2f size = getSizeAttribute(flowingRiverNode);
		body.rect.setSize(size);
		const sf::Vector2f pushForce(getProperty(flowingRiverNode, "pushForceX").toFloat(), getProperty(flowingRiverNode, "pushForceY").toFloat());
		PH_ASSERT_CRITICAL(pushForce.x == 0.f || pushForce.y == 0.f, "We don't support diagonal flowing rivers! - Either pushForceX or pushForceY must be zero.");
		pushingArea.pushForce = pushForce;
		const float particleAmountMultiplier = getProperty(flowingRiverNode, "particleAmountMultiplier").toFloat();
		particleEmitter.amountOfParticles = static_cast<unsigned>(particleAmountMultiplier * size.x * size.y / 100.f);
		particleEmitter.parInitialVelocity = pushForce;
		particleEmitter.parInitialVelocityRandom = pushForce;
		if(pushForce.y > 0.f) {
			particleEmitter.spawnPositionOffset = {0.f, 0.f};
			particleEmitter.randomSpawnAreaSize = {size.x, 0.f};
		}
		else if(pushForce.y < 0.f) {
			particleEmitter.spawnPositionOffset = {0.f, size.y};
			particleEmitter.randomSpawnAreaSize = {size.x, 0.f};
		}
		else if(pushForce.x > 0.f) {
			particleEmitter.spawnPositionOffset = {0.f, 0.f};
			particleEmitter.randomSpawnAreaSize = {0.f, size.y};
		}
		else if(pushForce.x < 0.f) {
			particleEmitter.spawnPositionOffset = {size.x, 0.f};
			particleEmitter.randomSpawnAreaSize = {0.f, size.y};
		}
		particleEmitter.parWholeLifetime = pushForce.x == 0.f ? std::abs(size.y / pushForce.y) : std::abs(size.x / pushForce.x);
	}

	void TiledParser::loadHealthComponent(const Xml& entityNode, entt::entity entity) const
	{
		auto& healthComponent = mGameRegistry.get<component::Health>(entity);
		healthComponent.healthPoints = getProperty(entityNode, "hp").toUnsigned();
		healthComponent.maxHealthPoints = getProperty(entityNode, "maxHp").toUnsigned();
	}

	void TiledParser::loadPosition(const Xml& entityNode, entt::entity entity) const
	{
		auto& bodyRect = mGameRegistry.get<component::BodyRect>(entity);
		bodyRect.rect.setPosition(getPositionAttribute(entityNode));
	}

	void TiledParser::loadSize(const Xml& entityNode, entt::entity entity) const
	{
		auto& bodyRect = mGameRegistry.get<component::BodyRect>(entity);
		bodyRect.rect.setSize(getSizeAttribute(entityNode));
	}

	void TiledParser::loadPositionAndSize(const Xml& entityNode, entt::entity entity) const
	{
		auto& bodyRect = mGameRegistry.get<component::BodyRect>(entity);
		bodyRect.rect.setPosition(getPositionAttribute(entityNode));
		bodyRect.rect.setSize(getSizeAttribute(entityNode));
	}

	void TiledParser::loadPositionAndOptionalSize(const Xml& entityNode, entt::entity entity) const
	{
		auto& bodyRect = mGameRegistry.get<component::BodyRect>(entity);
		bodyRect.rect.setPosition(getPositionAttribute(entityNode));
		if(auto size = getOptionalSizeAttribute(entityNode))
			bodyRect.rect.setSize(*size);
	}

	Xml TiledParser::getProperty(const Xml& objectNode, const std::string& propertyName) const
	{
		if (hasCustomProperty(objectNode, propertyName))
			return getCustomProperties(objectNode, propertyName);
		else
			return getDefaultProperties(objectNode.getAttribute("type")->toString(), propertyName);
	}

	bool TiledParser::hasCustomProperty(const Xml& gameObjectNode, const std::string& propertyName) const
	{
		if (const auto propertiesNode = gameObjectNode.getChild("properties")) {
			const std::vector<Xml> properties = propertiesNode->getChildren("property");
			for (const auto& property : properties)
				if (property.getAttribute("name")->toString() == propertyName)
					return true;
			return false;
		}
		return false;
	}

	Xml TiledParser::getCustomProperties(const Xml& gameObjectNode, const std::string& name) const
	{
		const auto propertiesNode = gameObjectNode.getChild("properties");
		auto properties = propertiesNode->getChildren("property");
		for (const auto& property : properties)
			if (property.getAttribute("name")->toString() == name)
				return *property.getAttribute("value");
		return Xml();
	}

	Xml TiledParser::getDefaultProperties(const std::string& objectName, const std::string& propertyName) const
	{
		const std::vector<Xml> objectNodes = getObjectTypeNodes();
		for (const auto& objectNode : objectNodes)
		{
			if (objectNode.getAttribute("name")->toString() == objectName) {
				std::vector<Xml> propertiesNodes = objectNode.getChildren("property");
				for (const auto& propertyNode : propertiesNodes)
					if (propertyNode.getAttribute("name")->toString() == propertyName)
						return *propertyNode.getAttribute("default");
			}
		}
		return Xml();
	}

	std::vector<Xml> TiledParser::getObjectTypeNodes() const
	{
		Xml objectTypesFile;
		objectTypesFile.loadFromFile("scenes/map/objecttypes.xml");
		const auto objectTypesNode = objectTypesFile.getChild("objecttypes");
		std::vector<Xml> getObjectTypeNodes = objectTypesNode->getChildren("objecttype");
		return getObjectTypeNodes;
	}

	sf::Vector2f TiledParser::getPositionAttribute(const Xml& DrawableGameObjectNode) const
	{
		return sf::Vector2f(
			DrawableGameObjectNode.getAttribute("x")->toFloat(),
			DrawableGameObjectNode.getAttribute("y")->toFloat()
		);
	}

	sf::Vector2f TiledParser::getSizeAttribute(const Xml& DrawableGameObjectNode) const
	{
		return sf::Vector2f(
			DrawableGameObjectNode.getAttribute("width")->toFloat(),
			DrawableGameObjectNode.getAttribute("height")->toFloat()
		);
	}

	// TODO_xml: Refactor this size getting mess

	std::optional<sf::Vector2f> TiledParser::getOptionalSizeAttribute(const Xml& gameObjectNode) const
	{
		auto width = gameObjectNode.getAttribute("width");
		auto height = gameObjectNode.getAttribute("height");
		if(width && height)
			return sf::Vector2f(width->toFloat(), height->toFloat());
		else
			return std::nullopt;
	}
} 
