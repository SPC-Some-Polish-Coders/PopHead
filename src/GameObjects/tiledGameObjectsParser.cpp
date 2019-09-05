#include "tiledGameObjectsParser.hpp"
#include "NonDrawableGameObjects/entrance.hpp"
#include "NonDrawableGameObjects/spawner.hpp"
#include "DrawableGameObjects/Characters/npc.hpp"
#include "DrawableGameObjects/Characters/Npcs/crawlingNpc.hpp"
#include "DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "DrawableGameObjects/Characters/player.hpp"
#include "DrawableGameObjects/car.hpp"
#include "DrawableGameObjects/gate.hpp"
#include "DrawableGameObjects/lever.hpp"
#include "GameObjectContainers/gameObjectLayers.hpp"
#include "GameObjectContainers/particlesSystem.hpp"
#include "GameObjectContainers/itemsContainer.hpp"
#include "gameObject.hpp"
#include "Scenes/cutSceneManager.hpp"
#include "Scenes/CutScenes/startGameCutscene.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

TiledGameObjectsParser::TiledGameObjectsParser(GameData* gameData, GameObject& root, CutSceneManager& cutSceneManager)
	:mGameData(gameData)
	,mRoot(root)
	,mCutSceneManager(cutSceneManager)
	,mHasLoadedPlayer(false)
{
}

void TiledGameObjectsParser::parseFile(const std::string& filePath) const
{
	PH_LOG_INFO("Game objects file (" + filePath + ") is being parsed.");

	Xml mapFile;
	mapFile.loadFromFile(filePath);

	const Xml mapNode = mapFile.getChild("map");
	const Xml gameObjects = findGameObjects(mapNode);
	if (gameObjects.toString() == "")
		return;
	
	loadObjects(gameObjects);

	mGameData->getAIManager().setIsPlayerOnScene(mHasLoadedPlayer);
}

Xml TiledGameObjectsParser::findGameObjects(const Xml& mapNode) const
{
	const std::vector<Xml> objectGroupNodes = mapNode.getChildren("objectgroup");
	for(const auto& objectGroupNode : objectGroupNodes)
	{
		if ((objectGroupNode.hasAttribute("name") && objectGroupNode.getAttribute("name").toString() == "gameObjects"))
			return objectGroupNode;
	}
	return Xml();
}

void TiledGameObjectsParser::loadObjects(const Xml& gameObjectsNode) const
{
	const std::vector<Xml> objects = gameObjectsNode.getChildren("object");

	loadLayerObjects();
	loadContainerObjects();

	for (const auto& gameObjectNode : objects) 
	{
		if (isObjectOfType(gameObjectNode, "Zombie")) loadZombie(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Npc")) loadNpc(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Spawner")) loadSpawner(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Entrance")) loadEntrance(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Camera")) loadCamera(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Player")) loadPlayer(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Car")) loadCar(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Gate")) loadGate(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Lever")) loadLever(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "CutScene")) loadCutScene(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "CrawlingNpc")) loadCrawlingNpc(gameObjectNode);
		else PH_LOG_ERROR("The type of object in map file (" + gameObjectNode.getAttribute("type").toString() + ") is unknown!");
	}
}

void TiledGameObjectsParser::loadLayerObjects() const
{
	mRoot.addChild(std::make_unique<GameObject>("LAYER_invisibleObjects"));
	mRoot.addChild(std::make_unique<LyingGameObjectsLayer>());
	mRoot.addChild(std::make_unique<StandingGameObjectsLayer>(mGameData->getPhysicsEngine()));
}

void TiledGameObjectsParser::loadContainerObjects() const
{
	auto& standingObjects = mRoot.getChild("LAYER_standingObjects");
	standingObjects.addChild(std::make_unique<ParticlesSystem>(mGameData->getRenderer()));
	standingObjects.addChild(std::make_unique<ItemsContainer>());
}

bool TiledGameObjectsParser::isObjectOfType(const Xml& gameObjectNode, const std::string& typeName) const
{
	return gameObjectNode.getAttribute("type").toString() == typeName;
}

void TiledGameObjectsParser::loadZombie(const Xml& zombieNode) const
{
	auto zombie = std::make_unique<Zombie>(mGameData);
	zombie->setPosition(getPositionAttribute(zombieNode));
	zombie->setHp(getProperty(zombieNode, "hp").toInt());
	zombie->setMaxHp(getProperty(zombieNode, "maxHp").toUnsigned());
	mGameData->getTextures().load("textures/others/bulletGround.png");

	auto& standingObjects = mRoot.getChild("LAYER_standingObjects");
	standingObjects.addChild(std::move(zombie));
}

void TiledGameObjectsParser::loadNpc(const Xml& npcNode) const
{
	auto npc = std::make_unique<Npc>(mGameData);

	npc->setPosition(getPositionAttribute(npcNode));
	npc->setHp(getProperty(npcNode, "hp").toInt());
	npc->setMaxHp(getProperty(npcNode, "maxHp").toUnsigned());
	const std::string texturePath = "textures/characters/" + getProperty(npcNode, "textureFileName").toString();
	npc->getSprite().setTexture(mGameData->getTextures().get(texturePath));

	auto& standingObjects = mRoot.getChild("LAYER_standingObjects");
	standingObjects.addChild(std::move(npc));
}

void TiledGameObjectsParser::loadSpawner(const Xml& spawnerNode) const
{
	auto spawner = std::make_unique<Spawner>(
		mGameData, "spawner",
		Cast::toObjectType(getProperty(spawnerNode, "spawnType").toString()),
		sf::seconds(getProperty(spawnerNode, "spawnFrequency").toFloat()),
		getPositionAttribute(spawnerNode)
	);

	auto& invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
	invisibleGameObjects.addChild(std::move(spawner));
}

void TiledGameObjectsParser::loadEntrance(const Xml& entranceNode) const
{
	const std::string scenePathRelativeToMapFile = getProperty(entranceNode, "gotoScene").toString();
	const std::string sceneFileName = *getSceneFileName(scenePathRelativeToMapFile);
	const std::string scenePathFromResources = "scenes/" + sceneFileName;

	auto entrance = std::make_unique<Entrance>(
		mGameData->getSceneManager(),
		scenePathFromResources,
		"entrance",
		getSizeAttribute(entranceNode),
		getPositionAttribute(entranceNode)
	);

	auto& invisibleGameObjects = mRoot.getChild("LAYER_invisibleObjects");
	invisibleGameObjects.addChild(std::move(entrance));
}

std::optional<std::string> TiledGameObjectsParser::getSceneFileName(const std::string& scenePathRelativeToMapFile) const
{
	std::size_t beginOfFileName = scenePathRelativeToMapFile.find_last_of('/');
	if(beginOfFileName == std::string::npos)
		return std::nullopt;
	return scenePathRelativeToMapFile.substr(beginOfFileName, scenePathRelativeToMapFile.size());
}

void TiledGameObjectsParser::loadGate(const Xml& gateNode) const
{
	auto& texture = mGameData->getTextures().get("textures/others/gate.png");
	auto gate = std::make_unique<Gate>(texture, getPositionAttribute(gateNode), mGameData->getPhysicsEngine(), false);
	auto& lyingObjects = mRoot.getChild("LAYER_lyingObjects");
	lyingObjects.addChild(std::move(gate));
}

void TiledGameObjectsParser::loadLever(const Xml& leverNode) const
{
	auto& texture = mGameData->getTextures().get("textures/others/lever.png");
	auto lever = std::make_unique<Lever>(texture);
	lever->setPosition(getPositionAttribute(leverNode));
	auto& lyingObjects = mRoot.getChild("LAYER_lyingObjects");
	lyingObjects.addChild(std::move(lever));
}

void TiledGameObjectsParser::loadCar(const Xml& carNode) const
{
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

	auto& standingObjects = mRoot.getChild("LAYER_standingObjects");
	standingObjects.addChild(std::move(car));
}

void TiledGameObjectsParser::loadCamera(const Xml& cameraNode) const
{
	const sf::Vector2f cameraTopLeftCornerPosition = getPositionAttribute(cameraNode);
	const sf::Vector2f cameraViewSize = getSizeAttribute(cameraNode);
	const sf::FloatRect cameraBounds(
		cameraTopLeftCornerPosition.x, cameraTopLeftCornerPosition.y,
		cameraViewSize.x, cameraViewSize.y
	);
	const sf::Vector2f cameraCenter = Math::getCenter(cameraBounds);
	
	auto& camera = mGameData->getRenderer().getCamera();
	camera.setSize(cameraViewSize);
	camera.setCenter(cameraCenter);
}

void TiledGameObjectsParser::loadPlayer(const Xml& playerNode) const
{
	auto player = std::make_unique<Player>(mGameData);
	player->getSprite().setTexture(mGameData->getTextures().get("textures/characters/playerFullAnimation.png"));
	auto playerPosition = getPositionAttribute(playerNode);
	player->setPosition(playerPosition);

	auto& standingObjects = mRoot.getChild("LAYER_standingObjects");
	standingObjects.addChild(std::move(player));

	mHasLoadedPlayer = true;
}

void TiledGameObjectsParser::loadCutScene(const Xml& cutSceneNode) const
{
	if(!getProperty(cutSceneNode, "isStartingCutSceneOnThisMap").toBool()) //temporary
		return;

	const std::string name = getProperty(cutSceneNode, "name").toString();

	if(name == "startGameCutScene") {
		auto startGameCutScene = std::make_unique<StartGameCutScene>(
			mRoot,
			mGameData->getRenderer().getCamera(),
			mGameData->getSoundPlayer(),
			mGameData->getMusicPlayer(),
			mGameData->getGui(),
			mGameData
		);
		mCutSceneManager.setMapStaringCutScene(std::move(startGameCutScene));
	}
}

void TiledGameObjectsParser::loadCrawlingNpc(const Xml& crawlingNpcNode) const
{
	auto crawlingNpc = std::make_unique<CrawlingNpc>(mGameData);
	crawlingNpc->setPosition(getPositionAttribute(crawlingNpcNode));
	mRoot.addChild(std::move(crawlingNpc));
}

Xml TiledGameObjectsParser::getProperty(const Xml& objectNode, const std::string& propertyName) const
{
	if (hasCustomProperty(objectNode, propertyName))
		return getCustomProperties(objectNode, propertyName);
	else
		return getDefaultProperties(objectNode.getAttribute("type").toString(), propertyName);
}

bool TiledGameObjectsParser::hasCustomProperty(const Xml& gameObjectNode, const std::string& propertyName) const
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

Xml TiledGameObjectsParser::getCustomProperties(const Xml& gameObjectNode, const std::string& name) const
{
	const Xml propertiesNode = gameObjectNode.getChild("properties");
	auto properties = propertiesNode.getChildren("property");
	for (const auto& property : properties)
		if (property.getAttribute("name").toString() == name)
			return property.getAttribute("value");

	return Xml();
}

Xml TiledGameObjectsParser::getDefaultProperties(const std::string& objectName, const std::string& propertyName) const
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

std::vector<Xml> TiledGameObjectsParser::getObjectTypeNodes() const
{
	Xml objectTypesFile;
	objectTypesFile.loadFromFile("scenes/map/objecttypes.xml");
	const Xml objectTypesNode = objectTypesFile.getChild("objecttypes");
	std::vector<Xml> getObjectTypeNodes = objectTypesNode.getChildren("objecttype");
	return getObjectTypeNodes;
}

sf::Vector2f TiledGameObjectsParser::getPositionAttribute(const Xml& DrawableGameObjectNode) const
{
	return sf::Vector2f(
		DrawableGameObjectNode.getAttribute("x").toFloat(),
		DrawableGameObjectNode.getAttribute("y").toFloat()
	);
}

sf::Vector2f TiledGameObjectsParser::getSizeAttribute(const Xml& DrawableGameObjectNode) const
{
	return sf::Vector2f(
		DrawableGameObjectNode.getAttribute("width").toFloat(),
		DrawableGameObjectNode.getAttribute("height").toFloat()
	);
}

}
