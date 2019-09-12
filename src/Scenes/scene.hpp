#pragma once

#include "GameObjects/GameObject.hpp"
#include "cutSceneManager.hpp"
#include <SFML/System.hpp>
#include <memory>

namespace ph{

class GameData; 
class CutScene;

class Scene
{
public:
    Scene();
    void update(sf::Time delta);

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }
	GameObject& getRoot() { return *mRoot.get(); }
	CutSceneManager& getCutSceneManager() { return mCutSceneManager; }

private:
	CutSceneManager mCutSceneManager;
	std::unique_ptr<GameObject> mRoot;
    bool mPause;
};

}
