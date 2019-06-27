#pragma once

#include "Utilities/xml.hpp"
#include "World/Entity/entity.hpp"
#include <string>
#include <vector>

namespace ph{

class SceneParser
{
public:
	SceneParser(GameData* const, Entity&, const std::string);
private:
	void getResources();
	void loadTextures(Xml&);

	void makeScene();
	void loadMap(Xml&);
	void loadPlayer(Xml&);
	void loadNpcs(Xml&);
	void loadTestNpcs(Xml&);
	void loadEnemies(Xml&);
	void loadZombies(Xml&);

private:
	const std::string sourceName;
	Entity& mRoot;
	GameData* const mGameData;

};

}
