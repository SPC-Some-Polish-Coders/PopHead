#include "kinematicCollisionHandler.hpp"
#include "collisionBody.hpp"

using PopHead::Physics::KinematicCollisionHandler;
using PopHead::Physics::CollisionBody;

void KinematicCollisionHandler::handleKinematicCollision(CollisionBody* kinematicBodyFirst, CollisionBody* kinematicBodySecond)
{
    float mass1 = kinematicBodyFirst->getMass();
    float mass2 = kinematicBodySecond->getMass();

    if(mass1 == mass2)
        return;

    sf::Vector2f kPos1 = kinematicBodyFirst->getPosition() + sf::Vector2f(16, 16);
    sf::Vector2f kPos2 = kinematicBodySecond->getPosition() + sf::Vector2f(16, 16);

    sf::Vector2f sides = kPos1 - kPos2;

    sf::Vector2f vector;
    float sideC = sqrt(sides.x * sides.x  +  sides.y * sides.y);
    float div = sides.x / sideC;
    if(div < 0)
        div *= -1;

	vector.x = kPos1.x > kPos2.x ? div : -1 * div;
	vector.y = kPos1.y > kPos2.y ? 1 - div : -1 * (1 - div);

    float force;
    sf::Vector2f forceVector;
    if(mass1 > mass2)
    {
        force = (mass1-mass2) * 12.5;
        forceVector = sf::Vector2f(force * vector.x, force * vector.y);
        kinematicBodySecond->setForceVector(forceVector);
    }
    else if(mass2 > mass1)
    {
        force = (mass2 - mass1) * 12.5;
        forceVector = sf::Vector2f(force * vector.x, force * vector.y);
        kinematicBodyFirst->setForceVector(forceVector);
    }
}
