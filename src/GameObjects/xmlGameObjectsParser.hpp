#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace ph {

class GameData;
class GameObject;
class Xml;

class XmlGameObjectsParser
{
public:
	XmlGameObjectsParser(GameData* const gameData, GameObject& root);

	void parseFile(const std::string& fileName);
private:
	void loadEntrances(const Xml& rootNode);
	void loadParticlesSystem();
	void loadPlayer(const Xml& rootNode);
	void loadGroups(const Xml& rootNode);
	void loadNpcGroup(const Xml& npcGroupNode);
	void loadEnemiesGroup(const Xml& enemyGroupNode);
	void loadZombies(const std::vector<Xml>& zombieNodes);
	void loadSpawners(const Xml& spawnerGroupNode);

	auto getPositionAttribute(const Xml& DrawableGameObjectNode) const -> const sf::Vector2f;
	auto getSizeAttribute(const Xml& objectNode) const -> const sf::Vector2f;

private:
	GameData* mGameData;
	GameObject& mRoot;
};

}
