#pragma once

#include "GameObjects/GameObject.hpp"
#include "cutSceneManager.hpp"
#include "Events/event.hpp"
#include "playerStatus.hpp"
#include <SFML/System.hpp>
#include <memory>

namespace ph{

class GameData; 
class CutScene;
class Player;

class Scene
{
public:
    Scene();

	void handleEvent(const ph::Event&);
    void update(sf::Time delta);

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }
	GameObject& getRoot() { return *mRoot.get(); }
	CutSceneManager& getCutSceneManager() { return mCutSceneManager; }

	void setPlayerStatus(const PlayerStatus& status);
	PlayerStatus getPlayerStatus() const;

private:
	Player& getPlayer() const;

private:
	CutSceneManager mCutSceneManager;
	std::unique_ptr<GameObject> mRoot;
    bool mPause;
};

}
