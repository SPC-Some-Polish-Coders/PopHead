#pragma once

#include "Utilities/xml.hpp"
#include "World/Entity/entity.hpp"
#include <string>
#include <vector>

namespace ph{

class SceneParser
{
public:
	SceneParser(GameData* const, Entity& root, const std::string fileName);
private:
	void getResources(const Xml& sceneSourceCode);
	void loadTextures(const Xml& loadingNode);

	void makeScene(const Xml& sceneSourceCode);
	void loadMap(const Xml& rootNode);
	void loadPlayer(const Xml& rootNode);
	void loadNpcs(const Xml& rootNode);
	void loadTestNpcs(const Xml& npcGroupNode);
	void loadEnemies(const Xml& rootNode);
	void loadZombies(const Xml& enemiesGroupNode);

private:
	Entity& mRoot;
	GameData* const mGameData;

};

}
