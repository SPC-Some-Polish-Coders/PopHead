#include "sceneParser.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser, typename MusicParser>
SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser, MusicParser>
	::SceneParser(GameData* const gameData, GameObject& root, const std::string& sceneFileName)
{
	PH_LOG_INFO("Scene linking file (" + sceneFileName + ") is being parsed.");

	Xml sceneFile;
	sceneFile.loadFromFile(sceneFileName);
	const auto sceneLinksNode = *sceneFile.getChild("scenelinks");

	parse<ResourcesParser>(gameData, sceneLinksNode, "neededResources");
	parseGameObjects(gameData, root, sceneLinksNode);
	parse<MapParser>(gameData, sceneLinksNode, "map");
	parse<GuiParser>(gameData, sceneLinksNode, "gui");	
	parse<MusicParser>(gameData, sceneLinksNode, "music");
}

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser, typename MusicParser>
template<typename Parser>
void SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser, MusicParser>
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

template<typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser, typename MusicParser>
void SceneParser<GuiParser, MapParser, GameObjectsParser, ResourcesParser, MusicParser>
	::parseGameObjects(GameData* const gameData, GameObject& root, const Xml& sceneLinksNode)
{
	const auto gameObjectsNode = sceneLinksNode.getChildren("map");
	if (gameObjectsNode.size() == 1)
	{
		const std::string gameObjectsFileName = "scenes/map/" + gameObjectsNode[0].getAttribute("filename").toString();
		GameObjectsParser gameObjectsParser(gameData, root);
		gameObjectsParser.parseFile(gameObjectsFileName);
	}
}

}
