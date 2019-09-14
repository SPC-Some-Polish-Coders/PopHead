#include "sceneParser.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser, typename AudioParser>
SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser, AudioParser>
	::SceneParser(GameData* const gameData, GameObject& root, CutSceneManager& cutSceneManager, const std::string& sceneFileName)
{
	PH_LOG_INFO("Scene linking file (" + sceneFileName + ") is being parsed.");

	Xml sceneFile;
	sceneFile.loadFromFile(sceneFileName);
	const auto sceneLinksNode = sceneFile.getChild("scenelinks");

	parse<ResourcesParser>(gameData, sceneLinksNode, "neededResources");
	parseGameObjects(gameData, root, cutSceneManager, sceneLinksNode);
	parse<MapParser>(gameData, sceneLinksNode, "map");
	parse<GuiParser>(gameData, sceneLinksNode, "gui");	
	parse<AudioParser>(gameData, sceneLinksNode, "audio");
}

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser, typename AudioParser>
template<typename Parser>
void SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser, AudioParser>
	::parse(GameData* const gameData, const Xml& sceneLinksNode, const std::string& categoryName)
{
	const auto categoryNode = sceneLinksNode.getChildren(categoryName);
	if (categoryNode.size() == 1)
	{
		const std::string categoryFilePath = "scenes/" + categoryName + "/" + categoryNode[0].getAttribute("filename").toString();
		Parser categoryParser;
		categoryParser.parseFile(gameData, categoryFilePath);
	}
}

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser, typename AudioParser>
void SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser, AudioParser>
	::parseGameObjects(GameData* const gameData, GameObject& root, CutSceneManager& cutSceneManager, const Xml& sceneLinksNode)
{
	const auto gameObjectsNode = sceneLinksNode.getChildren("map");
	if (gameObjectsNode.size() == 1)
	{
		const std::string gameObjectsFileName = "scenes/map/" + gameObjectsNode[0].getAttribute("filename").toString();
		GameObjectsParser gameObjectsParser(gameData, root, cutSceneManager);
		gameObjectsParser.parseFile(gameObjectsFileName);
	}
}

}
