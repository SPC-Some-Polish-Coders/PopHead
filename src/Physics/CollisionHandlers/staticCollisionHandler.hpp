#pragma once

#include <SFML/Graphics.hpp>

#include "Utilities/rect.hpp"

namespace ph {

class CollisionBody;

class StaticCollisionHandler
{
public:
	void operator()(CollisionBody& kinematicBody, const CollisionBody& staticBody);

private:
	void init(CollisionBody& kinematicBody, const CollisionBody& staticBody);
	void makeKinematicBodyStickToStaticBody();
	bool isKinematicBodyCollidingOnAxisX() const;
	bool isKinematicBodyOnTheLeftOfTheStaticBody() const;
	void stickToLeft();
	void stickToRight();
	bool isKinematicBodyCollidingOnAxisY() const;
	bool isKinematicBodyUpOfTheStaticBody() const;
	void stickToTop();
	void stickToBottom();

private:
	CollisionBody* mKinematicBody;
	const CollisionBody* mStaticBody;
	
	FloatRect mKinematicBodyRect;
	FloatRect mStaticBodyRect;
	FloatRect mKinematicBodyPreviousRect;
	//sf::FloatRect mStaticBodyPreviousRect;
};


}
