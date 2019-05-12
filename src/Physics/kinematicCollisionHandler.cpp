#include "kinematicCollisionHandler.hpp"
#include "collisionBody.hpp"
#include <iostream>
using PopHead::Physics::KinematicCollisionHandler;
using PopHead::Physics::CollisionBody;

void KinematicCollisionHandler::handleKinematicCollision(CollisionBody* kinematicBodyFirst, CollisionBody* kinematicBodySecond)
{
    if(kinematicBodyFirst == nullptr || kinematicBodySecond == nullptr)
        return;
    float mass1 = kinematicBodyFirst->getMass();
    float mass2 = kinematicBodySecond->getMass();
    if(mass1 == mass2)
        return;

    sf::Vector2f kPos1;
    sf::Vector2f kPos2;
    kPos1 = kinematicBodyFirst->getPosition();
    kPos2 = kinematicBodySecond->getPosition();
    kPos1.x += 16;
    kPos1.y += 16;
    kPos2.x += 16;
    kPos2.y += 16;
    sf::Vector2f sides;
    sides.x =kPos1.x-kPos2.x;
    sides.y =kPos1.y-kPos2.y;
    sf::Vector2f vector;
    float sideC = sqrt(sides.x*sides.x+sides.y*sides.y);
    float div = sides.x/sideC;
    if(div < 0)
        div*=-1;
    if(kPos1.x > kPos2.x)
    {
        vector.x = 1*div;
    }
    else
    {
        vector.x = -1*div;
    }
    if(kPos1.y > kPos2.y)
    {
        vector.y = 1*(1-div);
    }
    else
    {
        vector.y = -1*(1-div);
    }
   // std::cout << vector.x << " " << vector.y << "\n";
    if(mass1 > mass2)
    {
        kinematicBodySecond->setForceVector(vector);
    }
    else
    {
        kinematicBodyFirst->setForceVector(vector);
    }


    float force;
    sf::Vector2f forceVector;
    if(mass1 > mass2)
    {
        force = (mass1-mass2)*12.5;
        forceVector = sf::Vector2f(force*vector.x,force*vector.y);
        kinematicBodySecond->setForceVector(forceVector);
    }
    else if(mass2 > mass1)
    {
        force = (mass2-mass1)*12.5;
        forceVector = sf::Vector2f(force*vector.x,force*vector.y);
        kinematicBodyFirst->setForceVector(forceVector);
    }
}
