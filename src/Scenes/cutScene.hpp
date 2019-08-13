#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class GameObject;

class CutScene
{
public:
	CutScene(GameObject& root) :mRoot(root) {}
	virtual void update(const sf::Time delta) = 0;

protected:
	GameObject& mRoot;
};

}