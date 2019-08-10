#include "GameObjects/tiledGameObjectsParser.hpp"
#include "GameObjects/NotDrawableGameObjects/entrance.hpp"
#include "GameObjects/NotDrawableGameObjects/spawner.hpp"
#include "DrawableGameObjects/Characters/npc.hpp"
#include "DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjectContainers/enemyContainer.hpp"
#include "GameObjectContainers/particlesSystem.hpp"
#include "Utilities/xml.hpp"
#include "Utilities/math.hpp"
#include "Logs/logs.hpp"
#include "gameObject.hpp"
#include "gameData.hpp"

namespace ph {

TiledGameObjectsParser::TiledGameObjectsParser(GameData* gameData, GameObject& root)
	:mGameData(gameData)
	,mRoot(root)
{
}

void TiledGameObjectsParser::parseFile(const std::string& filePath)
{
	PH_LOG_INFO("Game objects file (" + filePath + ") is being parsed.");

	std::vector<Xml> objectTypeNodes = getObjectTypeNodes();
	Xml mapFile;
	mapFile.loadFromFile(filePath);

	Xml mapNode = mapFile.getChild("map");
	Xml gameObjects = findGameObjects(mapNode);
	if (gameObjects.toString() == "")
		return;
	
	loadObjects(gameObjects);
}

std::vector<Xml> TiledGameObjectsParser::getObjectTypeNodes()
{
	Xml objectTypesFile;
	objectTypesFile.loadFromFile("scenes/map/objecttypes.xml");
	auto objectTypesNode = objectTypesFile.getChild("objecttypes");
	std::vector<Xml> getObjectTypeNodes = objectTypesNode.getChildren("objecttype");
	return getObjectTypeNodes;
}

Xml TiledGameObjectsParser::findGameObjects(const Xml& mapNode)
{
	std::vector<Xml> objectGroupNodes = mapNode.getChildren("objectgroup");
	for(const auto& objectGroupNode : objectGroupNodes)
	{
		if ((objectGroupNode.hasAttribute("name") && objectGroupNode.getAttribute("name").toString() == "gameObjects"))
			return objectGroupNode;
	}
	return Xml();
}

void TiledGameObjectsParser::loadObjects(const Xml& gameObjectsNode)
{
	std::vector<Xml> objects = gameObjectsNode.getChildren("object");

	mRoot.addChild(std::make_unique<EnemyContainer>(mGameData));
	mRoot.addChild(std::make_unique<ParticlesSystem>(mGameData->getRenderer()));

	for (const auto& gameObjectNode : objects) 
	{
		if (isObjectOfType(gameObjectNode, "Entrance")) loadEntrance(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Npc")) loadNpc(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Zombie")) loadZombie(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Spawner")) loadSpawner(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Player")) loadPlayer(gameObjectNode);
		else if (isObjectOfType(gameObjectNode, "Camera")) loadCamera(gameObjectNode);
		else PH_EXIT_GAME("The type of object in map file (" + gameObjectNode.getAttribute("type").toString() + ") is unknown!");
	}
}

bool TiledGameObjectsParser::isObjectOfType(const Xml& gameObjectNode, const std::string& typeName)
{
	return gameObjectNode.getAttribute("type").toString() == typeName;
}

void TiledGameObjectsParser::loadPlayer(const Xml& playerNode)
{
	auto player = std::make_unique<Player>(mGameData);
	player->getSprite().setTexture(mGameData->getTextures().get("textures/characters/playerFullAnimation.png"));
	auto playerPosition = getPositionAttribute(playerNode);
	player->setPosition(playerPosition);
	mRoot.addChild(std::move(player));
}

void TiledGameObjectsParser::loadEntrance(const Xml& entranceNode)
{
	auto entrance = std::make_unique<Entrance>(
		mGameData->getSceneMachine(),
		getProperty(entranceNode, "gotoScene").toString(),
		"entrance", getSizeAttribute(entranceNode),
		getPositionAttribute(entranceNode)
	);

	mRoot.addChild(std::move(entrance));
}

void TiledGameObjectsParser::loadNpc(const Xml& npcNode)
{
	auto npc = std::make_unique<Npc>(mGameData);

	npc->setPosition(getPositionAttribute(npcNode));
	npc->setHp(getProperty(npcNode, "hp").toInt());
	npc->setHp(getProperty(npcNode, "maxHp").toUnsigned());

	mRoot.addChild(std::move(npc));
}

void TiledGameObjectsParser::loadZombie(const Xml& zombieNode)
{
	auto& enemies = mRoot.getChild("enemy_container");
	auto zombie = std::make_unique<Zombie>(mGameData);

	zombie->setPosition(getPositionAttribute(zombieNode));
	zombie->setHp(getProperty(zombieNode, "hp").toInt());
	zombie->setMaxHp(getProperty(zombieNode, "maxHp").toUnsigned());

	enemies.addChild(std::move(zombie));
}

void TiledGameObjectsParser::loadSpawner(const Xml& spawnerNode)
{
	auto propertiesNode = spawnerNode.getChild("properties");
	auto spawner = std::make_unique<Spawner>(
		mGameData, "spawner", 
		Cast::toObjectType(getProperty(spawnerNode, "spawnType").toString()),
		sf::seconds(getProperty(spawnerNode, "spawnFrequency").toFloat()),
		getPositionAttribute(spawnerNode)
	);

	mRoot.addChild(std::move(spawner));
}

void TiledGameObjectsParser::loadCamera(const Xml& cameraNode)
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

Xml TiledGameObjectsParser::getProperty(const Xml& objectNode, const std::string& propertyName)
{
	if (hasCustomProperty(objectNode, propertyName))
		return getCustomProperties(objectNode, propertyName);
	else
		return getDefaultProperties(objectNode.getAttribute("type").toString(), propertyName);
}

bool TiledGameObjectsParser::hasCustomProperty(const Xml& gameObjectNode, const std::string& propertyName)
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

Xml TiledGameObjectsParser::getDefaultProperties(const std::string& objectName, const std::string& propertyName)
{
	std::vector<Xml> objectNodes = getObjectTypeNodes();
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

Xml TiledGameObjectsParser::getCustomProperties(const Xml& gameObjectNode, const std::string& name)
{
	auto propertiesNode = gameObjectNode.getChild("properties");
	auto properties = propertiesNode.getChildren("property");
	for (const auto& property : properties)
		if (property.getAttribute("name").toString() == name)
			return property.getAttribute("value");

	return Xml();
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
