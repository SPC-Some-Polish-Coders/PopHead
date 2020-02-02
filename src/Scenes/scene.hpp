#pragma once

#include "cutSceneManager.hpp"
#include "Events/event.hpp"
#include "playerStatus.hpp"
#include <entt/entity/registry.hpp>
#include "ECS/systemsQueue.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

namespace ph{

class CutScene;
class AIManager;
class Terminal;
class SceneManager;
class Texture;
class ThreadPool;

class Scene
{
public:
    Scene(AIManager&, SceneManager&, Texture& tilesetTexture, ThreadPool&);

	void handleEvent(Event);
    void update(float dt);

	CutSceneManager& getCutSceneManager() { return mCutSceneManager; }
	SystemsQueue& getSystemsQueue() { return mSystemsQueue; }

	void setPlayerStatus(const PlayerStatus& status);
	PlayerStatus getPlayerStatus();
	void setPlayerPosition(sf::Vector2f newPosition);

	entt::registry& getRegistry();

private:
	CutSceneManager mCutSceneManager;
	entt::registry mRegistry;
	SystemsQueue mSystemsQueue;
};

}
