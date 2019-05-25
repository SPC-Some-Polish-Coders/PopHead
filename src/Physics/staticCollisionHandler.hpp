#ifndef POPHEAD_PHYSICS_STATICCOLLISIONHANDLER_H_
#define POPHEAD_PHYSICS_STATICCOLLISIONHANDLER_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Physics {


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


}}

#endif // !POPHEAD_PHYSICS_STATICCOLLISIONHANDLER_H_

