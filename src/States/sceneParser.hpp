#pragma once

#include "Utilities/xml.hpp"
#include "World/Entity/entity.hpp"
#include <string>
#include <vector>

namespace ph{

class SceneParser
{
public:
	SceneParser(GameData* const, Entity&, const std::string fileName);
private:
	void getResources(const Xml& sceneSourceCode);
	void loadTextures(const Xml& loadingNode);

	void makeScene(const Xml& sceneSourceCode);
	void loadMap(const Xml&);
	void loadPlayer(const Xml&);
	void loadNpcs(const Xml&);
	void loadTestNpcs(const Xml&);
	void loadEnemies(const Xml&);
	void loadZombies(const Xml&);

private:
	Entity& mRoot;
	GameData* const mGameData;

};

}
