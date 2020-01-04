#include "sceneParser.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "Renderer/renderer.hpp"
#include "ECS/Systems/arcadeMode.hpp"

namespace ph {

template<typename GuiParser, typename MapParser, typename ObjectsParser, typename AudioParser, typename EnttParser>
SceneParser<GuiParser, MapParser, ObjectsParser, AudioParser, EnttParser>
	::SceneParser(GameData* const gameData, CutSceneManager& cutSceneManager, EntitiesTemplateStorage& templateStorage,
                  entt::registry& gameRegistry, const std::string& sceneFileName, TextureHolder& textureHolder, SystemsQueue& systemsQueue,
	              GUI& gui, MusicPlayer& musicPlayer, AIManager& aiManager)
{
	PH_LOG_INFO("Scene linking file (" + sceneFileName + ") is being parsed.");

	// TODO: place it somewhere else
	textureHolder.load("textures/map/FULL_DESERT_TILESET_WIP.png");

	Xml sceneFile;
	sceneFile.loadFromFile(sceneFileName);
	const auto sceneLinksNode = sceneFile.getChild("scenelinks");

	aiManager.setAIMode(AIMode::normal);

	parseEcsEntities(sceneLinksNode, gameData->getAIManager(), templateStorage, gameRegistry, textureHolder);
	parseMap(sceneLinksNode, gameData->getAIManager(), gameRegistry, templateStorage, textureHolder);
	parseMapObjects(sceneLinksNode, gameData->getAIManager(), gameRegistry, templateStorage, cutSceneManager, gameData->getSceneManager(), textureHolder);
	parse<GuiParser>(gameData, sceneLinksNode, "gui");	
	parseAudio(sceneLinksNode, gameData->getSoundPlayer(), gameData->getMusicPlayer());
	parseAmbientLight(sceneLinksNode);
	parseArcadeMode(sceneLinksNode, systemsQueue, gui, aiManager, musicPlayer, templateStorage);
}

template<typename GuiParser, typename MapParser, typename ObjectsParser, typename AudioParser, typename EnttParser>
template<typename Parser>
void SceneParser<GuiParser, MapParser, ObjectsParser, AudioParser, EnttParser>
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


template<typename GuiParser, typename MapParser, typename ObjectsParser, typename AudioParser, typename EnttParser>
void SceneParser<GuiParser, MapParser, ObjectsParser, AudioParser, EnttParser>
::parseEcsEntities(const Xml& sceneLinksNode, AIManager& aiManager, EntitiesTemplateStorage& templateStorage, entt::registry& gameRegistry,
                   TextureHolder& textureHolder)
{
	const auto entitiesNode = sceneLinksNode.getChildren("ecsObjects");
	if (entitiesNode.size() == 1)
	{
		const std::string entitiesFilePath = "scenes/ecs/" + entitiesNode[0].getAttribute("filename").toString();
		EnttParser parser;
		parser.parseFile(entitiesFilePath, templateStorage, gameRegistry, textureHolder);
		if (parser.loadedPlayer())
			aiManager.setIsPlayerOnScene(true);
		else
			aiManager.setIsPlayerOnScene(false);
	}
}

template<typename GuiParser, typename MapParser, typename ObjectsParser, typename AudioParser, typename EnttParser>
void SceneParser<GuiParser, MapParser, ObjectsParser, AudioParser, EnttParser>::parseAmbientLight(const Xml& sceneLinksNode)
{
	const auto ambientLightNode = sceneLinksNode.getChild("ambientLight");
	sf::Color color = ambientLightNode.getAttribute("color").toColor();
	Renderer::setAmbientLightColor(color);
}

template<typename GuiParser, typename MapParser, typename ObjectsParser, typename AudioParser, typename EnttParser>
inline void SceneParser<GuiParser, MapParser, ObjectsParser, AudioParser, EnttParser>
	::parseArcadeMode(const Xml& sceneLinksNode, SystemsQueue& systemsQueue, GUI& gui, AIManager& aiManager,
	                  MusicPlayer& musicPlayer, EntitiesTemplateStorage& templateStorage)
{
	if(!sceneLinksNode.getChildren("arcadeMode").empty())
		systemsQueue.appendSystem<system::ArcadeMode>(std::ref(gui), std::ref(aiManager), std::ref(musicPlayer), std::ref(templateStorage));
}

template<typename GuiParser, typename MapParser, typename ObjectsParser, typename AudioParser, typename EnttParser>
void SceneParser<GuiParser, MapParser, ObjectsParser, AudioParser, EnttParser>::parseMap(const Xml& sceneLinksNode, AIManager& aiManager, entt::registry& gameRegistry, EntitiesTemplateStorage& templates, TextureHolder& textures)
{
	const auto categoryNode = sceneLinksNode.getChildren("map");
	if (categoryNode.size() == 1)
	{
		const std::string filePath = "scenes/map/" + categoryNode[0].getAttribute("filename").toString();
		MapParser mapParser;
		mapParser.parseFile(filePath, aiManager, gameRegistry, templates, textures);
	}
}

template<typename GuiParser, typename MapParser, typename ObjectsParser, typename AudioParser, typename EnttParser>
void SceneParser<GuiParser, MapParser, ObjectsParser, AudioParser, EnttParser>
	::parseMapObjects(const Xml& sceneLinksNode, AIManager& aiManager, entt::registry& gameRegistry, EntitiesTemplateStorage& templates,
	                  CutSceneManager& cutSceneManager, SceneManager& sceneManager, TextureHolder& textureHolder)
{
	const auto categoryNode = sceneLinksNode.getChildren("map");
	if (categoryNode.size() == 1)
	{
		const std::string filePath = "scenes/map/" + categoryNode[0].getAttribute("filename").toString();
		ObjectsParser objectsParser(cutSceneManager, templates, gameRegistry, sceneManager, textureHolder);
		objectsParser.parseFile(filePath);
		if (objectsParser.hasLoadedPlayer())
			aiManager.setIsPlayerOnScene(true);
		else
			aiManager.setIsPlayerOnScene(false);
	}
}

template<typename GuiParser, typename MapParser, typename ObjectsParser, typename AudioParser, typename EnttParser>
void SceneParser<GuiParser, MapParser, ObjectsParser, AudioParser, EnttParser>::parseAudio(const Xml& sceneLinksNode, SoundPlayer& sound, MusicPlayer& music)
{
	const auto audioNode = sceneLinksNode.getChildren("audio");
	if (audioNode.size() == 1)
	{
		const std::string audioFilePath = "scenes/audio/" + audioNode[0].getAttribute("filename").toString();
		AudioParser audioParser;
		audioParser.parseFile(sound, music, audioFilePath);
	}
}

}
