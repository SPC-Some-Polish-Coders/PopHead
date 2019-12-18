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
class MusicPlayer;
class SoundPlayer;
class AIManager;
class Terminal;
class SceneManager;
class GUI;
class Texture;

class Scene
{
public:
    Scene(MusicPlayer&, SoundPlayer&, AIManager&, Terminal&, SceneManager&, GUI&, Texture& tilesetTexture);

	void handleEvent(const Event&);
    void update(sf::Time dt);

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }
	CutSceneManager& getCutSceneManager() { return mCutSceneManager; }

	void setPlayerStatus(const PlayerStatus& status);
	PlayerStatus getPlayerStatus() const;
	void setPlayerPosition(sf::Vector2f newPosition);

	entt::registry& getRegistry();

private:
	CutSceneManager mCutSceneManager;
	entt::registry mRegistry;
	SystemsQueue mSystemsQueue;
    bool mPause;
};

}
