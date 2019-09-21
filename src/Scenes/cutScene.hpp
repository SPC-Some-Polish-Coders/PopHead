#pragma once

#include <SFML/System.hpp>

namespace ph {

class GameObject;

class CutScene
{
public:
	CutScene(GameObject& root);
	virtual void update(const sf::Time delta) = 0;
	bool isActive() const;

protected:
	GameObject& mRoot;
	bool mIsActive;
};

}