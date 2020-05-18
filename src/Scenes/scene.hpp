#pragma once

#include "playerStatus.hpp"
#include <entt/entity/registry.hpp>
#include "ECS/systemsQueue.hpp"
#include "ECS/entitiesTemplateStorage.hpp"
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

namespace ph{

class AIManager;
class SceneManager;
class Texture;
class ThreadPool;

class Scene
{
public:
    Scene(AIManager&, SceneManager&, Texture& tilesetTexture, ThreadPool&, EntitiesTemplateStorage&);

	void handleEvent(sf::Event);
    void update(float dt);

	SystemsQueue& getSystemsQueue() { return mSystemsQueue; }

	void setPlayerStatus(const PlayerStatus& status);
	PlayerStatus getPlayerStatus();
	void setPlayerPosition(sf::Vector2f newPosition);

	entt::registry& getRegistry();

private:
	entt::registry mRegistry;
	SystemsQueue mSystemsQueue;
};

}
