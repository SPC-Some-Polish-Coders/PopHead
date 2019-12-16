#include <catch.hpp>

#include "Physics/CollisionHandlers/kinematicCollisionHandler.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"

namespace ph {

TEST_CASE("Kinematic collisions are handled properly when one body is way heavier", "[Physics][KinematicCollisoinHandler]")
{
	KinematicCollisionHandler kinematicCollisionHandler;
	CollisionBody heavierBody({0, 0, 10, 10}, 50);
	
	SECTION("When ligterBody is going to collide with heavierBody at 0 radians in unit circle") {
		CollisionBody lighterBody({11, 0, 10, 10}, 25);
		lighterBody.move({-2, 0});
		kinematicCollisionHandler(lighterBody, heavierBody);
		auto forceVector = lighterBody.getForceVector();
		CHECK(Approx(forceVector.x).margin(1) == 312.5);
		CHECK(forceVector.y == 0);
	}
	SECTION("When ligterBody is going to collide with heavierBody at something about 30 degrees in unit circle") {
		CollisionBody lighterBody({11, -6, 10, 10}, 25);
		lighterBody.move({-2, 0});
		kinematicCollisionHandler(lighterBody, heavierBody);
		auto forceVector = lighterBody.getForceVector();
		CHECK(Approx(forceVector.x).margin(2) == 260);
		CHECK(Approx(forceVector.y).margin(2) == -52);
	}
	SECTION("When ligterBody is going to collide with heavierBody at aomething about PI/4 radians in unit circle") {
		CollisionBody lighterBody({11, -9.99f, 10, 10}, 25);
		lighterBody.move({-1, 0});
		kinematicCollisionHandler(lighterBody, heavierBody);
		auto forceVector = lighterBody.getForceVector();
		CHECK(Approx(forceVector.x).margin(2) == 221);
		CHECK(Approx(forceVector.y).margin(2) == -91.5);
	}
	SECTION("When ligterBody is going to collide with heavierBody at PI/2 radians in unit circle") {
		CollisionBody lighterBody({0, -11, 10, 10}, 25);
		lighterBody.move({0, 2});
		kinematicCollisionHandler(lighterBody, heavierBody);
		auto forceVector = lighterBody.getForceVector();
		CHECK(forceVector.x == 0);
		CHECK(Approx(forceVector.y).margin(1) == -312.5);
	}
	SECTION("When ligterBody is going to collide with heavierBody at PI radians in unit circle") {
		CollisionBody lighterBody({-11, 0, 10, 10}, 25);
		lighterBody.move({2, 0});
		kinematicCollisionHandler(lighterBody, heavierBody);
		auto forceVector = lighterBody.getForceVector();
		CHECK(Approx(forceVector.x).margin(1) == -312.5);
		CHECK(forceVector.y == 0);
	}
	SECTION("When ligterBody is going to collide with heavierBody at 3PI/2 radians in unit circle") {
		CollisionBody lighterBody({0, 11, 10, 10}, 25);
		lighterBody.move({0, -2});
		kinematicCollisionHandler(lighterBody, heavierBody);
		auto forceVector = lighterBody.getForceVector();
		CHECK(forceVector.x == 0);
		CHECK(Approx(forceVector.y).margin(1) == 312.5);
	}
}

}