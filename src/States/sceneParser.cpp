#include "States/sceneParser.hpp"

#include "World/Entity/Objects/Characters/enemies/zombie.hpp"
#include "World/Entity/Objects/map.hpp"
#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/Characters/npc.hpp"

#include <iostream>

namespace ph {

SceneParser::SceneParser(GameData* const gameDatas, Entity& root, const std::string fileName)
	:sourceName(fileName)
	,mGameData(gameDatas)
	,mRoot(root) 
{
	getResources();
	makeScene();
}

void SceneParser::getResources()
{
	Xml sceneFile;
	sceneFile.loadFromFile(sourceName);
	Xml loadingNode = sceneFile.getChild("loading");
	loadTextures(loadingNode);
}

void SceneParser::loadTextures(Xml& loadingNode)
{

	Xml textureNode = loadingNode.getChild("textures");
	std::vector<Xml> textureNodes = textureNode.getChildren("resource");
	for (auto& node : textureNodes)
	{
		const std::string filename = node.getAttribute("respath").toString();
		mGameData->getTextures().load("textures/" + filename);
	}
}

void SceneParser::makeScene()
{
	Xml sceneFile;
	sceneFile.loadFromFile(sourceName);
	Xml rootNode = sceneFile.getChild("root");

	loadMap(rootNode);
	loadPlayer(rootNode);
	loadEnemies(rootNode);
	loadNpcs(rootNode);
}

void SceneParser::loadMap(Xml& rootNode)
{
	std::vector<Xml> rootNodes = rootNode.getChildren("map");
	auto map = std::make_unique<Map>(mGameData, rootNodes[0].getAttribute("name").toString()); 	
	//Presumed that map can be only one
	map->loadFromFile(rootNodes[0].getAttribute("filepath").toString());
	mRoot.addChild(std::move(map));
}

void SceneParser::loadPlayer(Xml& rootNode)
{
	std::vector<Xml> rootNodes = rootNode.getChildren("player");
	auto player = std::make_unique<Player>(mGameData);
	player->getSprite().setTexture(mGameData->getTextures().get(rootNodes[0].getAttribute("texturepath").toString()));
	//Presumed that player can be only one
	mRoot.addChild(std::move(player));
}

void SceneParser::loadNpcs(Xml& rootNode)
{
	std::vector<Xml> rootElements = rootNode.getChildren("group");
	for (auto& groupTypes : rootElements)
	{
		if (groupTypes.getAttribute("name").toString() == "npc")
			loadTestNpcs(groupTypes);
			//loadOtherNpcs(groupTypes); etc...
	}
}

void SceneParser::loadTestNpcs(Xml& npcGroupNode)
{
	std::vector<Xml> npcs = npcGroupNode.getChildren("npcTest");
	for (auto& npc : npcs)
	{
		auto npcTest = std::make_unique<Npc>(mGameData);
		npcTest->setPosition(sf::Vector2f(npc.getAttribute("positionX").toFloat(),
			npc.getAttribute("positionY").toFloat()));
		mRoot.addChild(std::move(npcTest));
	}
}

void SceneParser::loadEnemies(Xml& rootNode)
{
	std::vector<Xml> rootElements = rootNode.getChildren("group");
	for (auto& groupTypes : rootElements)
	{
		if (groupTypes.getAttribute("name").toString() == "enemies")
			loadZombies(groupTypes);
			//loadDevils(groupTypes); etc...
	}
}

void SceneParser::loadZombies(Xml& enemiesGroupNode)
{
	std::vector<Xml> enemies = enemiesGroupNode.getChildren("zombie");
	for (auto& enemy : enemies)
	{
		auto zombie = std::make_unique<Zombie>(mGameData);
		zombie->setPosition(sf::Vector2f(enemy.getAttribute("positionX").toFloat(),
			enemy.getAttribute("positionY").toFloat()));
		mRoot.addChild(std::move(zombie));
	}
}

}