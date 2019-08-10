#pragma once

#include "GameObjects/DrawableGameObject.hpp"

#include <SFML/System.hpp>

#include <memory>

namespace ph{

class GameData; 

class Scene
{
public:
    Scene();
    void input();
    void update(sf::Time delta);

	void setPause(bool pause) { mPause = pause; }
	bool getPause() const { return mPause; }

	GameObject& getRoot() { return *mRoot.get(); }

private:
	std::unique_ptr<GameObject> mRoot;
    bool mPause;
};

}
