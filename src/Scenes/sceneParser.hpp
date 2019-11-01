#pragma once

#include <string>

namespace ph{

class GameData;
class Xml;
class CutSceneManager;

template <typename GuiParser, typename MapParser/*, typename GameObjectsParser*/, typename ResourcesParser, typename AudioParser>
class SceneParser
{
public:
	SceneParser(GameData* const gameData/*, GameObject& root*/, CutSceneManager& cutSceneManager, const std::string& sceneFileName);

private:
	template<typename Parser>
	void parse(GameData* const gameData, const Xml& sceneLinksNode, const std::string& categoryName);

	void parseGameObjects(GameData* const gameData/*, GameObject& root*/, CutSceneManager& cutSceneManager, const Xml& sceneLinksNode);
};

}
#include "sceneParser.inl"
