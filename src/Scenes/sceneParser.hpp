#pragma once

#include "ECS/entitiesTemplateStorage.hpp"

#include <entt/entity/registry.hpp>

#include <string>

namespace ph{

class GameData;
class Xml;
class CutSceneManager;

template <typename GuiParser, typename MapParser, typename ResourcesParser, typename AudioParser, typename EnttParser>
class SceneParser
{
public:
	SceneParser(GameData* const gameData, CutSceneManager& cutSceneManager, EntitiesTemplateStorage& templateStorage, entt::registry& gameRegistry, const std::string& sceneFileName);

private:
	template<typename Parser>
	void parse(GameData* const gameData, const Xml& sceneLinksNode, const std::string& categoryName);

	void parseEcsEntities(const Xml& sceneLinksNode, EntitiesTemplateStorage& templateStorage, entt::registry& gameRegistry);
	//void parseGameObjects(GameData* const gameData, CutSceneManager& cutSceneManager, const Xml& sceneLinksNode);
};

}
#include "sceneParser.inl"
