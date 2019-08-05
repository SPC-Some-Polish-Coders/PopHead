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
	};
}

#include "sceneParser.inl"
