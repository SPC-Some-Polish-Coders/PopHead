#include "xmlGameObjectsParser.hpp"
#include "GameObjects/DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "GameObjects/DrawableGameObjects/Characters/npc.hpp"
#include "GameObjects/NotDrawableGameObjects/entrance.hpp"
#include "GameObjects/NotDrawableGameObjects/spawner.hpp"
#include "GameObjects/GameObjectContainers/enemyContainer.hpp"
#include "GameObjects/GameObjectContainers/particlesSystem.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

XmlGameObjectsParser::XmlGameObjectsParser(GameData* const gameData, GameObject& root)
	:mGameData(gameData)
	,mRoot(root)
{
}

void XmlGameObjectsParser::parseFile(const std::string& fileName)
{
	PH_LOG_INFO("Game objects file (" + fileName + ") is beign parsed.");

	Xml gameObjectsFile;
	gameObjectsFile.loadFromFile(fileName);
	const Xml rootNode = gameObjectsFile.getChild("root");

	loadEntrances(rootNode);
	loadParticlesSystem();
	loadPlayer(rootNode);
	loadGroups(rootNode);

	mGameData->getRenderer().getCamera().setCenter({0, 0});
}

void XmlGameObjectsParser::loadEntrances(const Xml& rootNode)
{
	const std::vector<Xml> entranceNodes = rootNode.getChildren("entrance");
	for (const auto& entranceNode : entranceNodes)
	{
		const std::string filepath = entranceNode.getAttribute("filepath").toString();
		const std::string name = entranceNode.getAttribute("name").toString();
		auto entrance = std::make_unique<Entrance>(mGameData->getSceneMachine(), filepath, name, getSizeAttribute(entranceNode), getPositionAttribute(entranceNode));
		mRoot.addChild(std::move(entrance));
	}
}

void XmlGameObjectsParser::loadParticlesSystem()
{
	mRoot.addChild(std::make_unique<ParticlesSystem>(mGameData->getRenderer()));
}

void XmlGameObjectsParser::loadPlayer(const Xml& rootNode)
{
	const Xml playerNode = rootNode.getChild("player");
	auto player = std::make_unique<Player>(mGameData);
	player->getSprite().setTexture(mGameData->getTextures().get(playerNode.getAttribute("texturepath").toString()));
	mRoot.addChild(std::move(player));
}

void XmlGameObjectsParser::loadGroups(const Xml& rootNode)
{
	const std::vector<Xml> groupNodes = rootNode.getChildren("group");
	for(const auto& groupNode : groupNodes) {
		const std::string groupName = groupNode.getAttribute("name").toString();
		if(groupName == "npc")
			loadNpcGroup(groupNode);
		else if(groupName == "enemies")
			loadEnemiesGroup(groupNode);
		else if(groupName == "spawners")
			loadSpawners(groupNode);
		else
			PH_EXCEPTION("Syntax error: There is not such group: " + groupName);
	}
}

void XmlGameObjectsParser::loadNpcGroup(const Xml& npcGroupNode)
{
	const std::vector<Xml> npcNodes = npcGroupNode.getChildren("npcTest");
	for (const auto& npcNode : npcNodes) {
		auto npcTest = std::make_unique<Npc>(mGameData);
		npcTest->setPosition(getPositionAttribute(npcNode));
		mRoot.addChild(std::move(npcTest));
	}
}

void XmlGameObjectsParser::loadEnemiesGroup(const Xml& enemyGroupNode)
{
	mRoot.addChild(std::make_unique<EnemyContainer>(mGameData));
	const std::vector<Xml> zombieNodes = enemyGroupNode.getChildren("zombie");
	loadZombies(zombieNodes);
}

void XmlGameObjectsParser::loadZombies(const std::vector<Xml>& zombieNodes)
{
	auto& enemies = mRoot.getChild("enemy_container");
	for (const auto& zombieNode : zombieNodes) {
		auto zombie = std::make_unique<Zombie>(mGameData);
		zombie->setPosition(getPositionAttribute(zombieNode));
		enemies.addChild(std::move(zombie));
	}
}

void XmlGameObjectsParser::loadSpawners(const Xml& spawnerGroupNode)
{
	const std::vector<Xml> spawnerNodes = spawnerGroupNode.getChildren("spawner");
	for (const auto& spawnerNode : spawnerNodes)
	{
		const float frequency = spawnerNode.getAttribute("frequency").toFloat();
		const std::string name = spawnerNode.getAttribute("name").toString();
		auto objectSpawner = std::make_unique<Spawner>(
			mGameData, name + "Spawner", Cast::toObjectType(name), sf::seconds(frequency), getPositionAttribute(spawnerNode)
		);
		mRoot.addChild(std::move(objectSpawner));
	}
}

auto XmlGameObjectsParser::getPositionAttribute(const Xml& DrawableGameObjectNode) const -> const sf::Vector2f
{
	return sf::Vector2f(
		DrawableGameObjectNode.getAttribute("positionX").toFloat(),
		DrawableGameObjectNode.getAttribute("positionY").toFloat()
	);
}

auto XmlGameObjectsParser::getSizeAttribute(const Xml& objectNode) const -> const sf::Vector2f
{
	return sf::Vector2f(
		objectNode.getAttribute("width").toFloat(),
		objectNode.getAttribute("height").toFloat()
	);
}

}