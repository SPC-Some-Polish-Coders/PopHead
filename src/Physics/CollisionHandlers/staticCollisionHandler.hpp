#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class CollisionBody;

class StaticCollisionHandler
{
public:
	void operator()(CollisionBody* kinematicBody, CollisionBody* staticBody);

private:
	void init(CollisionBody* kinematicBody, CollisionBody* staticBody);
	void makeKinematicBodyStickToStaticBody();
	bool isKinematicBodyCollidingOnAxisX();
	bool isKinematicBodyOnTheLeftOfTheStaticBody();
	void stickToRight();
	void stickToLeft();
	bool isKinematicBodyUpOfTheStaticBody();
	void stickToTop();
	void stickToBottom();

private:
	CollisionBody* mKinematicBody;
	CollisionBody* mStaticBody;
	
	sf::FloatRect mKinematicBodyRect;
	sf::FloatRect mStaticBodyRect;
	sf::FloatRect mKinematicBodyPreviousRect;
	sf::FloatRect mStaticBodyPreviousRect;
};


}
