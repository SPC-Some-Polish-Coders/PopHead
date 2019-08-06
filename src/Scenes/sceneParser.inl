#include "sceneParser.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser>
SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser>
	::SceneParser(GameData* const gameData, GameObject& root, const std::string& sceneFileName)
{
	PH_LOG_INFO("Scene linking file (" + sceneFileName + ") is beign parsed.");

	Xml sceneFile;
	sceneFile.loadFromFile(sceneFileName);
	const auto sceneLinksNode = sceneFile.getChild("scenelinks");

	parse<ResourcesParser>(gameData, sceneLinksNode, "neededResources");
	parseGameObjects(gameData, root, sceneLinksNode);
	parse<MapParser>(gameData, sceneLinksNode, "map");
	parse<GuiParser>(gameData, sceneLinksNode, "gui");
}

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser>
template<typename Parser>
void SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser>
	::parse(GameData* const gameData, const Xml& sceneLinksNode, const std::string& categoryName)
{
	const auto categoryNode = sceneLinksNode.getChild(categoryName);
	const std::string categoryFilePath = "scenes/" + categoryName + "/" + categoryNode.getAttribute("filename").toString();
	Parser categoryParser;
	categoryParser.parseFile(gameData, categoryFilePath);
}

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser>
void SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser>
	::parseGameObjects(GameData* const gameData, GameObject& root, const Xml& sceneLinksNode)
{
	const auto gameObjectsNode = sceneLinksNode.getChild("gameObjects");
	const std::string gameObjectsFileName = "scenes/gameObjects/" + gameObjectsNode.getAttribute("filename").toString();
	GameObjectsParser gameObjectsParser(gameData, root);
	gameObjectsParser.parseFile(gameObjectsFileName);
}

}