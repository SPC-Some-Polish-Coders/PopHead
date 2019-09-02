#pragma once

#include <SFML/System.hpp>

namespace ph {

class GameObject;

class CutScene
{
public:
	CutScene(GameObject& root) :mRoot(root) ,mIsActive(true) {}
	virtual void input() = 0;
	virtual void update(const sf::Time delta) = 0;
	bool isActive() const { return mIsActive; }	

protected:
	GameObject& mRoot;
	bool mIsActive;
};

}