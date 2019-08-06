#pragma once

#include <string>

namespace ph{

class GameData;
class Xml;

template <typename GuiParser, typename MapParser, typename GameObjectsParser, typename ResourcesParser>
class SceneParser
{
public:
	SceneParser(GameData* const gameData, GameObject& root, const std::string& sceneFileName);

private:
	template<typename Parser>
	void parse(GameData* const gameData, const Xml& sceneLinksNode, const std::string& categoryName);

	void parseGameObjects(GameData* const gameData, GameObject& root, const Xml& sceneLinksNode);
};

}
#include "sceneParser.inl"
