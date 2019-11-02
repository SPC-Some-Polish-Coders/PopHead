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

class Scene
{
public:
    explicit Scene(sf::RenderWindow& window);

	void handleEvent(const Event&);
    void update(sf::Time delta);

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }
	CutSceneManager& getCutSceneManager() { return mCutSceneManager; }

	void setPlayerStatus(const PlayerStatus& status);
	PlayerStatus getPlayerStatus() const;

private:
	void initiateSystemsQueue(sf::RenderWindow& window);

private:
	CutSceneManager mCutSceneManager;

	entt::registry mRegistry;
	SystemsQueue mSystemsQueue;

    bool mPause;
};

}
