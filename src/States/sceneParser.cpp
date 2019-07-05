#include "States/sceneParser.hpp"

#include "World/Entity/Objects/Characters/enemies/zombie.hpp"
#include "World/Entity/Objects/Map/map.hpp"
#include "World/Entity/Objects/Characters/player.hpp"
#include "World/Entity/Objects/Characters/npc.hpp"

#include <iostream>

namespace ph {

SceneParser::SceneParser(GameData* const gameData, Entity& root, const std::string fileName)
	:mGameData(gameData)
	,mRoot(root) 
{
	Xml sceneSourceCode;
	sceneSourceCode.loadFromFile(fileName);

	loadResources(sceneSourceCode);
	loadMusic(sceneSourceCode);
	loadScene(sceneSourceCode);
}

void SceneParser::loadResources(const Xml& sceneSourceCode)
{
	const Xml loadingNode = sceneSourceCode.getChild("loading");
	loadTextures(loadingNode);
}

void SceneParser::loadTextures(const Xml& loadingNode)
{
	const Xml textureNode = loadingNode.getChild("textures");
	const std::vector<Xml> textureNodes = textureNode.getChildren("resource");
	for (const auto& node : textureNodes) {
		const std::string filename = node.getAttribute("respath").toString();
		mGameData->getTextures().load("textures/" + filename);
	}
}

void SceneParser::loadMusic(const Xml& sceneSourceCode)
{
	const Xml musicNode = sceneSourceCode.getChild("music");
	const Xml startThemeNode = musicNode.getChild("theme");
	const std::string themeFileName = startThemeNode.getAttribute("filename").toString();
	const std::string themeFilePath = "music/" + themeFileName;
	mGameData->getMusicPlayer().play(themeFilePath);
}

void SceneParser::loadScene(const Xml& sceneSourceCode)
{
	const Xml rootNode = sceneSourceCode.getChild("root");

	loadMap(rootNode);
	loadPlayer(rootNode);
	loadGroups(rootNode);
}

void SceneParser::loadMap(const Xml& rootNode)
{
	const Xml mapNode = rootNode.getChild("map");
	auto map = std::make_unique<Map>(mGameData, mapNode.getAttribute("name").toString()); 	
	map->loadFromFile(mapNode.getAttribute("filepath").toString());
	mRoot.addChild(std::move(map));
}

void SceneParser::loadPlayer(const Xml& rootNode)
{
	const Xml playerNode = rootNode.getChild("player");
	auto player = std::make_unique<Player>(mGameData);
	player->getSprite().setTexture(mGameData->getTextures().get(playerNode.getAttribute("texturepath").toString()));
	mRoot.addChild(std::move(player));
}

void SceneParser::loadGroups(const Xml& rootNode)
{
	const std::vector<Xml> groupNodes = rootNode.getChildren("group");
	for(const auto& groupNode : groupNodes) {
		const std::string groupName = groupNode.getAttribute("name").toString();
		if(groupName == "npc")
			loadNpcGroup(groupNode);
		else if(groupName == "enemies")
			loadEnemiesGroup(groupNode);
		else if(groupName == "spawners")
			loadSpawnersGroup(groupNode);
		else
			PH_EXCEPTION("Syntax error: There is not such group: " + groupName);
	}
}

void SceneParser::loadNpcGroup(const Xml& npcGroupNode)
{
	const std::vector<Xml> npcNodes = npcGroupNode.getChildren("npcTest");
	for (const auto& npcNode : npcNodes) {
		auto npcTest = std::make_unique<Npc>(mGameData);
		npcTest->setPosition(getPositionAttribute(npcNode));
		mRoot.addChild(std::move(npcTest));
	}
}

void SceneParser::loadEnemiesGroup(const Xml& enemyGroupNode)
{
	const std::vector<Xml> zombieNodes = enemyGroupNode.getChildren("zombie");
	loadZombies(zombieNodes);
}

void SceneParser::loadZombies(const std::vector<Xml>& zombieNodes)
{
	for (const auto& zombieNode : zombieNodes) {
		auto zombie = std::make_unique<Zombie>(mGameData);
		zombie->setPosition(getPositionAttribute(zombieNode));
		mRoot.addChild(std::move(zombie));
	}
}

void SceneParser::loadSpawnersGroup(const Xml& spawnerGroupNode)
{
	// TODO: Implement this method when Spawner class will be ready.
}

auto SceneParser::getPositionAttribute(const Xml& objectNode) const -> const sf::Vector2f
{
	return sf::Vector2f(
		objectNode.getAttribute("positionX").toFloat(),
		objectNode.getAttribute("positionY").toFloat()
	);
}

}