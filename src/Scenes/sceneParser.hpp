#pragma once

#include "Utilities/xml.hpp"
#include "GameObjects/DrawableGameObject.hpp"

#include <string>
#include <vector>

namespace ph{

class GameData;

template <typename GuiParser, typename MapParser, typename GameObjectsParser>
class SceneParser
{
public:
	SceneParser(GameData* const gameData, GameObject& root, const std::string& sceneFileName);

private:
	void parseMap(GameData* const gameData, const Xml& sceneLinksNode);
	void parseGameObjects(GameData* const gameData, GameObject& root, const Xml& sceneLinksNode);
	void parseGui(GameData* const gameData, const Xml& sceneLinksNode);
};

}
#include "sceneParser.inl"
