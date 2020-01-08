#include "sceneParser.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "Renderer/renderer.hpp"
#include "ECS/Systems/arcadeMode.hpp"
#include "Events/actionEventManager.hpp"
#include "ECS/entitiesParser.hpp"
#include "ECS/tiledParser.hpp"
#include "ECS/xmlMapParser.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "Audio/xmlAudioParser.hpp"

namespace ph {

void parseScene(GameData* const gameData, CutSceneManager& cutSceneManager, EntitiesTemplateStorage& templateStorage,
                entt::registry& gameRegistry, const std::string& sceneFileName, TextureHolder& textureHolder, SystemsQueue& systemsQueue,
                GUI& gui, MusicPlayer& musicPlayer, AIManager& aiManager)
{
	PH_LOG_INFO("Scene linking file (" + sceneFileName + ") is being parsed.");

	// TODO: place it somewhere else
	textureHolder.load("textures/map/FULL_DESERT_TILESET_WIP.png");

	// TODO: We don't want to exit game if we misspelled something in gotoscene terminal command
	Xml sceneFile;
	PH_ASSERT_CRITICAL(sceneFile.loadFromFile(sceneFileName), "scene file \"" + sceneFileName + "\" wasn't loaded correctly!");
	const auto sceneLinksNode = *sceneFile.getChild("scenelinks");

	aiManager.setAIMode(AIMode::normal);
	aiManager.setIsPlayerOnScene(false);
	ActionEventManager::setAllActionsEnabled(true);

	// parse ecs entities
	if(const auto entitiesNode = sceneLinksNode.getChild("ecsObjects")) {
		const std::string entitiesFilePath = "scenes/ecs/" + entitiesNode->getAttribute("filename")->toString();
		EntitiesParser parser;
		parser.parseFile(entitiesFilePath, templateStorage, gameRegistry, textureHolder);
	}

	// parse map
	if(const auto mapNode = sceneLinksNode.getChild("map")) {
		Xml map;
		const std::string mapFilepath = "scenes/map/" + mapNode->getAttribute("filename")->toString();
		PH_ASSERT_CRITICAL(map.loadFromFile(mapFilepath), "map file \"" + mapFilepath + "\" wasn't loaded correctly!");
		map = *map.getChild("map");
		XmlMapParser mapParser;
		mapParser.parseFile(map, aiManager, gameRegistry, templateStorage, textureHolder);
		TiledParser tiledParser(cutSceneManager, templateStorage, gameRegistry, gameData->getSceneManager(), textureHolder);
		tiledParser.parseFile(map);
		aiManager.setIsPlayerOnScene(tiledParser.hasLoadedPlayer());
	}

	// parse gui
	if(const auto guiNode = sceneLinksNode.getChild("gui")) {
		const std::string categoryFilePath = "scenes/gui/" + guiNode->getAttribute("filename")->toString();
		XmlGuiParser categoryParser;
		categoryParser.parseFile(gameData, categoryFilePath);
	}

	// parse audio
	if(const auto audioNode = sceneLinksNode.getChild("audio")) {
		const std::string audioFilePath = "scenes/audio/" + audioNode->getAttribute("filename")->toString();
		XmlAudioParser audioParser;
		audioParser.parseFile(gameData->getSoundPlayer(), gameData->getMusicPlayer(), audioFilePath);
	}

	// parse ambient light 
	const auto ambientLightNode = sceneLinksNode.getChild("ambientLight");
	const sf::Color color = ambientLightNode->getAttribute("color")->toColor();
	Renderer::setAmbientLightColor(color);

	// parse arcade mode
	if(!sceneLinksNode.getChildren("arcadeMode").empty())
		systemsQueue.appendSystem<system::ArcadeMode>(std::ref(gui), std::ref(aiManager), std::ref(musicPlayer), std::ref(templateStorage));
}

}

