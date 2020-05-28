#pragma once

#include "playerStatus.hpp"
#include <entt/entity/registry.hpp>
#include "ECS/systemsQueue.hpp"
#include "ECS/entitiesTemplateStorage.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

namespace ph{

class AIManager;
class SceneManager;
class Texture;
class ThreadPool;

class Scene
{
public:
    Scene(AIManager&, SceneManager&, Texture& tilesetTexture, ThreadPool&, EntitiesTemplateStorage&, sf::Window*);

	void handleEvent(sf::Event);
    void update(float dt);

	SystemsQueue& getSystemsQueue() { return mSystemsQueue; }

	void setPlayerStatus(const PlayerStatus& status);
	PlayerStatus getPlayerStatus();
	void setPlayerPosition(Vec2 newPosition);

	entt::registry& getRegistry();

private:
	entt::registry mRegistry;
	SystemsQueue mSystemsQueue;
};

}
