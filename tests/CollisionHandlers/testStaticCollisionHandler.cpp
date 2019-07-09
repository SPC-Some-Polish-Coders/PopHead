#include <catch.hpp>

#include "Physics/CollisionHandlers/staticCollisionHandler.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"
#include "EntityComponentSystem/Objects/Characters/player.hpp"
#include "gameData.hpp"

namespace ph {

TEST_CASE("static collision after straight movement", "[Physics][StaticCollisionHandler]") 
{
	StaticCollisionHandler staticCollisionHandler;
	
	CollisionBody kinematicBody({0, 0, 10, 10}, 25);
	kinematicBody.actionsAtTheEndOfPhysicsLoopIteration();

	SECTION("after moving right") {
		kinematicBody.move({10, 0});
		CollisionBody staticBody({15, -10, 20, 30}, 0);
		staticCollisionHandler(kinematicBody, staticBody);
		CHECK(Approx(kinematicBody.getPosition().x).margin(0.001f) == 5.f);
		CHECK(Approx(kinematicBody.getPosition().y).margin(0.001f) == 0.f);
	}
	SECTION("after moving left") {
		kinematicBody.move({-10, 0});
		CollisionBody staticBody({-25, -10, 20, 30}, 0);
		staticCollisionHandler(kinematicBody, staticBody);
		CHECK(Approx(kinematicBody.getPosition().x).margin(0.001f) == -5.f);
		CHECK(Approx(kinematicBody.getPosition().y).margin(0.001f) == 0.f);
	}
	SECTION("after moving down") {
		kinematicBody.move({0, 10});
		CollisionBody staticBody({-10, 15, 30, 20}, 0);
		staticCollisionHandler(kinematicBody, staticBody);
		CHECK(Approx(kinematicBody.getPosition().x).margin(0.001f) == 0.f);
		CHECK(Approx(kinematicBody.getPosition().y).margin(0.001f) == 5.f);
	}
	SECTION("after moving up") {
		kinematicBody.move({0, -10});
		CollisionBody staticBody({-10, -25, 30, 20}, 0);
		staticCollisionHandler(kinematicBody, staticBody);
		CHECK(Approx(kinematicBody.getPosition().x).margin(0.001f) == 0.f);
		CHECK(Approx(kinematicBody.getPosition().y).margin(0.001f) == -5.f);
	}
}

}
