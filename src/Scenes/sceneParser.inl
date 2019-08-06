//#include "sceneParser.hpp"
//
//#include "Map/map.hpp"
//#include "GameObjects/DrawableGameObjects/Characters/Enemies/zombie.hpp"
//#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
//#include "GameObjects/DrawableGameObjects/Characters/npc.hpp"
//#include "GameObjects/NotDrawableGameObjects/entrance.hpp"
//#include "GameObjects/NotDrawableGameObjects/spawner.hpp"
//#include "GameObjects/GameObjectContainers/enemyContainer.hpp"
//#include "GameObjects/GameObjectContainers/particlesSystem.hpp"

#include "sceneParser.hpp"
#include "gameData.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"

namespace ph {

	template<typename GuiParser, typename MapParser, typename GameObjectsParser>
	SceneParser<GuiParser, MapParser, GameObjectsParser>::SceneParser(GameData* const gameData, GameObject& root, const std::string& sceneFileName)
	{
		PH_LOG_INFO("Scene linking file (" + sceneFileName + ") is beign parsed.");
		Xml sceneFile;
		sceneFile.loadFromFile(sceneFileName);
		const auto sceneLinksNode = sceneFile.getChild("scenelinks");
		parseMap(gameData, sceneLinksNode);
		parseGameObjects(gameData, root, sceneLinksNode);
		parseGui(gameData, sceneLinksNode);
	}

	template<typename GuiParser, typename MapParser, typename GameObjectsParser>
	inline void SceneParser<GuiParser, MapParser, GameObjectsParser>::parseMap(GameData* const gameData, const Xml& sceneLinksNode)
	{
		const auto mapNode = sceneLinksNode.getChild("map");
		const std::string mapFileName = mapNode.getAttribute("filename").toString();
		MapParser mapParser;
		mapParser.parseFile(gameData, mapFileName);
	}

	template<typename GuiParser, typename MapParser, typename GameObjectsParser>
	inline void SceneParser<GuiParser, MapParser, GameObjectsParser>::parseGameObjects(GameData* const gameData, GameObject& root, const Xml& sceneLinksNode)
	{
		const auto gameObjectsNode = sceneLinksNode.getChild("gameObjects");
		const std::string gameObjectsFileName = gameObjectsNode.getAttribute("filename").toString();
		GameObjectsParser gameObjectsParser;
		gameObjectsParser.parseFile(gameData, root, gameObjectsFileName);
	}

	template<typename GuiParser, typename MapParser, typename GameObjectsParser>
	inline void SceneParser<GuiParser, MapParser, GameObjectsParser>::parseGui(GameData* const gameData, const Xml& sceneLinksNode)
	{
		const auto guiNode = sceneLinksNode.getChild("gui");
		const std::string guiFileName = guiNode.getAttribute("filename").toString();
		GuiParser guiParser;
		guiParser.parseFile(gameData, guiFileName);
	}
}


//SceneParser::SceneParser(GameData* const gameData, GameObject& root, const std::string fileName)
//	:mGameData(gameData)
//	,mRoot(root) 
//{
//	Xml sceneSourceCode;
//	sceneSourceCode.loadFromFile(fileName);
//	const Xml sceneNode = sceneSourceCode.getChild("scene");
//
//	loadResources(sceneNode);
//	loadMusic(sceneNode);
//	loadScene(sceneNode);
//
//	mGameData->getRenderer().getCamera().setCenter({0, 0});
//}
//
//void SceneParser::loadResources(const Xml& sceneNode)
//{
//	const Xml loadingNode = sceneNode.getChild("loading");
//	loadTextures(loadingNode);
//}
//
//void SceneParser::loadTextures(const Xml& loadingNode)
//{
//	const Xml textureNode = loadingNode.getChild("textures");
//	const std::vector<Xml> textureNodes = textureNode.getChildren("resource");
//	for (const auto& node : textureNodes) {
//		const std::string filename = node.getAttribute("respath").toString();
//		mGameData->getTextures().load("textures/" + filename);
//	}
//}
//
//void SceneParser::loadMusic(const Xml& sceneNode)
//{
//	const Xml musicNode = sceneNode.getChild("music");
//	const Xml startThemeNode = musicNode.getChild("theme");
//	const std::string themeFileName = startThemeNode.getAttribute("filename").toString();
//	const std::string themeFilePath = "music/" + themeFileName;
//	mGameData->getMusicPlayer().play(themeFilePath);
//}
//
//void SceneParser::loadScene(const Xml& sceneNode)
//{
//	const Xml rootNode = sceneNode.getChild("root");
//
//	loadMap(rootNode);
//	loadEntrances(rootNode);
//	loadParticlesSystem();
//	loadPlayer(rootNode);
//	loadGroups(rootNode);
//}
//
//void SceneParser::loadMap(const Xml& rootNode)
//{
//	const Xml mapNode = rootNode.getChild("map");
//	auto& map = mGameData->getMap();
//	map.loadFromFile(mapNode.getAttribute("filepath").toString());
//}
//
//void SceneParser::loadEntrances(const Xml& rootNode)
//{
//	const std::vector<Xml> entranceNodes = rootNode.getChildren("entrance");
//	for (const auto& entranceNode : entranceNodes)
//	{
//		const std::string filepath = entranceNode.getAttribute("filepath").toString();
//		const std::string name = entranceNode.getAttribute("name").toString();
//		auto entrance = std::make_unique<Entrance>(mGameData->getSceneMachine(), filepath, name, getSizeAttribute(entranceNode), getPositionAttribute(entranceNode));
//		mRoot.addChild(std::move(entrance));
//	}
//}
//
//void SceneParser::loadParticlesSystem()
//{
//	mRoot.addChild(std::make_unique<ParticlesSystem>(mGameData->getRenderer()));
//}
//
//void SceneParser::loadPlayer(const Xml& rootNode)
//{
//	const Xml playerNode = rootNode.getChild("player");
//	auto player = std::make_unique<Player>(mGameData);
//	player->getSprite().setTexture(mGameData->getTextures().get(playerNode.getAttribute("texturepath").toString()));
//	mRoot.addChild(std::move(player));
//}
//
//void SceneParser::loadGroups(const Xml& rootNode)
//{
//	const std::vector<Xml> groupNodes = rootNode.getChildren("group");
//	for(const auto& groupNode : groupNodes) {
//		const std::string groupName = groupNode.getAttribute("name").toString();
//		if(groupName == "npc")
//			loadNpcGroup(groupNode);
//		else if(groupName == "enemies")
//			loadEnemiesGroup(groupNode);
//		else if(groupName == "spawners")
//			loadSpawners(groupNode);
//		else
//			PH_EXCEPTION("Syntax error: There is not such group: " + groupName);
//	}
//}
//
//void SceneParser::loadNpcGroup(const Xml& npcGroupNode)
//{
//	const std::vector<Xml> npcNodes = npcGroupNode.getChildren("npcTest");
//	for (const auto& npcNode : npcNodes) {
//		auto npcTest = std::make_unique<Npc>(mGameData);
//		npcTest->setPosition(getPositionAttribute(npcNode));
//		mRoot.addChild(std::move(npcTest));
//	}
//}
//
//void SceneParser::loadEnemiesGroup(const Xml& enemyGroupNode)
//{
//	mRoot.addChild(std::make_unique<EnemyContainer>(mGameData));
//	const std::vector<Xml> zombieNodes = enemyGroupNode.getChildren("zombie");
//	loadZombies(zombieNodes);
//}
//
//void SceneParser::loadZombies(const std::vector<Xml>& zombieNodes)
//{
//	auto& enemies = mRoot.getChild("enemy_container");
//	for (const auto& zombieNode : zombieNodes) {
//		auto zombie = std::make_unique<Zombie>(mGameData);
//		zombie->setPosition(getPositionAttribute(zombieNode));
//		enemies.addChild(std::move(zombie));
//	}
//}
//
//void SceneParser::loadSpawners(const Xml& spawnerGroupNode)
//{
//	const std::vector<Xml> spawnerNodes = spawnerGroupNode.getChildren("spawner");
//	for (const auto& spawnerNode : spawnerNodes)
//	{
//		const float frequency = spawnerNode.getAttribute("frequency").toFloat();
//		const std::string name = spawnerNode.getAttribute("name").toString();
//		auto objectSpawner = std::make_unique<Spawner>(
//			mGameData, name + "Spawner", Cast::toObjectType(name), sf::seconds(frequency), getPositionAttribute(spawnerNode)
//		);
//		mRoot.addChild(std::move(objectSpawner));
//	}
//}
//
//auto SceneParser::getPositionAttribute(const Xml& DrawableGameObjectNode) const -> const sf::Vector2f
//{
//	return sf::Vector2f(
//		DrawableGameObjectNode.getAttribute("positionX").toFloat(),
//		DrawableGameObjectNode.getAttribute("positionY").toFloat()
//	);
//}
//
//auto SceneParser::getSizeAttribute(const Xml& objectNode) const -> const sf::Vector2f
//{
//	return sf::Vector2f(
//		objectNode.getAttribute("width").toFloat(),
//		objectNode.getAttribute("height").toFloat()
//	);
//}
