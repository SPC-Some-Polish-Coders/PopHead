#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <optional>

namespace ph {

class GameData;
class GameObject;
class Xml;

class TiledGameObjectsParser
{
public:
	TiledGameObjectsParser(GameData* gameData, GameObject& root);

	void parseFile(const std::string& filePath);

private:
	std::vector<Xml> getObjectTypeNodes();
	Xml findGameObjects(const Xml& mapFile);
	Xml getProperty(const Xml& objectNode, const std::string& propertyName);
	Xml getDefaultProperties(const std::string& name, const std::string& propertyName);
	Xml getCustomProperties(const Xml& gameObjectNode, const std::string& name);
	bool hasCustomProperty(const Xml& gameObjectNode, const std::string& propertyName);

	void loadObjects(const Xml& gameObjects);
	bool isObjectOfType(const Xml& gameObjectNode, const std::string& typeName);
	void loadEntrance(const Xml& entranceNode);
	void loadZombie(const Xml& zombieNode);
	void loadSpawner(const Xml& spawnerNode);
	void loadNpc(const Xml& npcNode);
	void loadCamera(const Xml& cameraNode);

	void loadParticlesSystem();
	void loadPlayer(const Xml& rootNode);

	sf::Vector2f getPositionAttribute(const Xml& gameObjectNode) const;
	sf::Vector2f getSizeAttribute(const Xml& gameObjectNode) const;

private:
	GameData* const mGameData;
	GameObject& mRoot;
};

}