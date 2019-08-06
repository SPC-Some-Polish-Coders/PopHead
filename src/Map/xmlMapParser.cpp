#include "xmlMapParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

	void XmlMapParser::parseFile(GameData* const gameData, const std::string& fileName)
	{
		PH_LOG_INFO("Map file (" + fileName + ") is beign parsed.");

		// This is very very temporary! Just to make map load.
		gameData->getTextures().load("textures/map/FULL_DESERT_TILESET_WIP.png");

		// It is only temporary solution!
		// Later the whole map file will be parsed right here and the data will be passed to map class.
		auto& map = gameData->getMap();
		map.loadFromFile(fileName);
	}
}
