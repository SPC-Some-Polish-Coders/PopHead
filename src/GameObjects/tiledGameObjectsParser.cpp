#include "GameObjects/tiledGameObjectsParser.hpp"
#include "Utilities/xml.hpp"
#include "GameObjects/NotDrawableGameObjects/entrance.hpp"
#include "GameObjects/NotDrawableGameObjects/spawner.hpp"
#include "DrawableGameObjects/Characters/npc.hpp"
#include "DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjectContainers/enemyContainer.hpp"
#include "GameObjectContainers/particlesSystem.hpp"
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

	Xml mapNode = *mapFile.getChild("map");
	Xml gameObjects = findGameObjects(mapNode);
	if (gameObjects.toString() == "")
		return;
	
	loadObjects(gameObjects);

	mGameData->getRenderer().getCamera().setCenter({0, 0});
}

std::vector<Xml> TiledGameObjectsParser::getObjectTypeNodes()
{
	Xml objectTypesFile;
	objectTypesFile.loadFromFile("scenes/map/objecttypes.xml");
	auto objectTypesNode = *objectTypesFile.getChild("objecttypes");
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

	//Temporary load--
	auto player = std::make_unique<Player>(mGameData);
	player->getSprite().setTexture(mGameData->getTextures().get("textures/characters/vaultManSheet.png"));
	mRoot.addChild(std::move(player));

	mRoot.addChild(std::make_unique<EnemyContainer>(mGameData));
	mRoot.addChild(std::make_unique<ParticlesSystem>(mGameData->getRenderer()));
	//----------------

	for (const auto& gameObjectNode : objects)
	{
		if (gameObjectNode.getAttribute("type").toString() == "Entrance") loadEntrance(gameObjectNode);
		else if (gameObjectNode.getAttribute("type").toString() == "Npc") loadNpc(gameObjectNode);
		else if (gameObjectNode.getAttribute("type").toString() == "Zombie") loadZombie(gameObjectNode);
		else if (gameObjectNode.getAttribute("type").toString() == "Spawner") loadSpawner(gameObjectNode);
		else PH_EXIT_GAME("The type of object in map file (" + gameObjectNode.getAttribute("type").toString() + ") is unknown!");
	}
}

void TiledGameObjectsParser::loadEntrance(const Xml& entranceNode)
{
	auto entrance = std::make_unique<Entrance>(mGameData->getSceneMachine(), "somePath", "entrance", getSizeAttribute(entranceNode), getPositionAttribute(entranceNode));
	mRoot.addChild(std::move(entrance));
}

void TiledGameObjectsParser::loadNpc(const Xml& npcNode)
{
	auto npcTest = std::make_unique<Npc>(mGameData);
	npcTest->setPosition(getPositionAttribute(npcNode));
	mRoot.addChild(std::move(npcTest));
}

void TiledGameObjectsParser::loadZombie(const Xml& zombieNode)
{
	auto& enemies = mRoot.getChild("enemy_container");
	auto zombie = std::make_unique<Zombie>(mGameData);
	zombie->setPosition(getPositionAttribute(zombieNode));
	enemies.addChild(std::move(zombie));
}

void TiledGameObjectsParser::loadSpawner(const Xml& spawnerNode)
{
	auto propertiesNode = spawnerNode.getChild("properties");

	auto spawner = propertiesNode ?
	std::make_unique<Spawner>(mGameData, "spawner", ObjectType::Zombie,
		sf::seconds(getProperties(spawnerNode, "spawnFrequency").toFloat()), getPositionAttribute(spawnerNode))
	: std::make_unique<Spawner>(mGameData, "spawner", ObjectType::Zombie,
		sf::seconds(getDefaultProperties("Spawner", "spawnFrequency").toFloat()),getPositionAttribute(spawnerNode));

	mRoot.addChild(std::move(spawner));
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

Xml TiledGameObjectsParser::getProperties(const Xml& gameObjectNode, const std::string& name)
{
	Xml propertiesNode = *gameObjectNode.getChild("properties");
	std::vector<Xml> properties = propertiesNode.getChildren("property");
	for (const auto& property : properties)
	{
		if (property.getAttribute("name").toString() == name)
			return property.getAttribute("value");
	}
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