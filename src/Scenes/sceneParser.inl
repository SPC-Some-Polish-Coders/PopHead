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
		parseGameObjects(gameData, root, sceneLinksNode);
		parseMap(gameData, sceneLinksNode);
		parseGui(gameData, sceneLinksNode);
	}

	template<typename GuiParser, typename MapParser, typename GameObjectsParser>
	inline void SceneParser<GuiParser, MapParser, GameObjectsParser>::parseMap(GameData* const gameData, const Xml& sceneLinksNode)
	{
		const auto mapNode = sceneLinksNode.getChild("map");
		const std::string mapFilePath = "scenes/maps/" + mapNode.getAttribute("filename").toString();
		MapParser mapParser;
		mapParser.parseFile(gameData, mapFilePath);
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