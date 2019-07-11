#pragma once

#include <SFML/Graphics.hpp>

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
	void stickToRight();
	void stickToLeft();
	bool isKinematicBodyUpOfTheStaticBody() const;
	void stickToTop();
	void stickToBottom();

private:
	CollisionBody* mKinematicBody;
	const CollisionBody* mStaticBody;
	
	sf::FloatRect mKinematicBodyRect;
	sf::FloatRect mStaticBodyRect;
	sf::FloatRect mKinematicBodyPreviousRect;
	sf::FloatRect mStaticBodyPreviousRect;
};


}
