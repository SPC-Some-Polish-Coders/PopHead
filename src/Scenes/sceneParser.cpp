#include "sceneParser.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include "Renderer/renderer.hpp"
#include "ECS/Systems/arcadeMode.hpp"
#include "ECS/entitiesParser.hpp"
#include "ECS/tiledParser.hpp"
#include "ECS/xmlMapParser.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "Audio/xmlAudioParser.hpp"
#include <thread>
#include "Utilities/profiling.hpp"

namespace ph {

void parseScene(CutSceneManager& cutSceneManager, EntitiesTemplateStorage& templateStorage,
                entt::registry& gameRegistry, const std::string& sceneFileName, SystemsQueue& systemsQueue,
                AIManager& aiManager, SceneManager& sceneManager)
{
	PH_PROFILE_FUNCTION(0);

	PH_LOG_INFO("Scene linking file (" + sceneFileName + ") is being parsed.");

	// TODO: We don't want to exit game if we misspelled something in gotoscene terminal command
	Xml sceneFile;
	PH_ASSERT_CRITICAL(sceneFile.loadFromFile(sceneFileName), "scene file \"" + sceneFileName + "\" wasn't loaded correctly!");
	const auto sceneLinksNode = *sceneFile.getChild("scenelinks");

	aiManager.setAIMode(AIMode::normal);
	aiManager.setIsPlayerOnScene(false);

	// parse gui
	if(const auto guiNode = sceneLinksNode.getChild("gui")) {
		const std::string filepath = "scenes/gui/" + guiNode->getAttribute("filename")->toString();
		XmlGuiParser guiParser;
		guiParser.parseGuiXml(filepath);
	}

	// parse audio
	if(const auto audioNode = sceneLinksNode.getChild("audio")) {
		const std::string audioFilePath = "scenes/audio/" + audioNode->getAttribute("filename")->toString();
		parseAudioXmlFile(audioFilePath);
	}

	// parse ambient light 
	const auto ambientLightNode = sceneLinksNode.getChildren("ambientLight");
	if (const auto ambientLightNode = sceneLinksNode.getChild("ambientLight")) {
		sf::Color color = ambientLightNode->getAttribute("color")->toColor();
		Renderer::setAmbientLightColor(color);
	}
	else
		Renderer::setAmbientLightColor(sf::Color(255, 255, 255));

	// parse arcade mode
	if(!sceneLinksNode.getChildren("arcadeMode").empty())
		systemsQueue.appendSystem<system::ArcadeMode>(std::ref(aiManager), std::ref(templateStorage));

	// parse ecs entities
	templateStorage.clearStorage();
	if(const auto entitiesNode = sceneLinksNode.getChild("ecsObjects")) {
		const std::string entitiesFilePath = "scenes/ecs/" + entitiesNode->getAttribute("filename")->toString();
		EntitiesParser parser;
		parser.parseFile(entitiesFilePath, templateStorage, gameRegistry);
	}

	// parse map
	if(const auto mapNode = sceneLinksNode.getChild("map")) {
		Xml map;
		const std::string mapFilepath = "scenes/map/" + mapNode->getAttribute("filename")->toString();
		PH_ASSERT_CRITICAL(map.loadFromFile(mapFilepath), "map file \"" + mapFilepath + "\" wasn't loaded correctly!");
		map = *map.getChild("map");
		XmlMapParser mapParser;
		mapParser.parseFile(map, aiManager, gameRegistry, templateStorage);
		TiledParser tiledParser(cutSceneManager, templateStorage, gameRegistry, sceneManager);
		tiledParser.parseFile(map);
		aiManager.setIsPlayerOnScene(tiledParser.hasLoadedPlayer());
	}
}

}

