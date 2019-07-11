#include <catch.hpp>

#include "Physics/physicsEngine.hpp"
#include <vector>

namespace ph {

TEST_CASE("static bodies can be created and are accessible by returned reference")
{
	PhysicsEngine physicsEngine;
	
	auto& b1 = physicsEngine.createStaticBodyAndGetTheReference({1, 1, 1, 1});
	REQUIRE(b1.getRect() == sf::FloatRect(1, 1, 1, 1));

	auto& b2 = physicsEngine.createStaticBodyAndGetTheReference({2, 2, 1, 1});
	REQUIRE(b1.getRect() == sf::FloatRect(1, 1, 1, 1));
	REQUIRE(b2.getRect() == sf::FloatRect(2, 2, 1, 1));
	
	auto& b3 = physicsEngine.createStaticBodyAndGetTheReference({3, 3, 1, 1});
	REQUIRE(b1.getRect() == sf::FloatRect(1, 1, 1, 1));
	REQUIRE(b2.getRect() == sf::FloatRect(2, 2, 1, 1));
	REQUIRE(b3.getRect() == sf::FloatRect(3, 3, 1, 1));
	
	auto& b4 = physicsEngine.createStaticBodyAndGetTheReference({4, 4, 1, 1});
	REQUIRE(b1.getRect() == sf::FloatRect(1, 1, 1, 1));
	REQUIRE(b2.getRect() == sf::FloatRect(2, 2, 1, 1));
	REQUIRE(b3.getRect() == sf::FloatRect(3, 3, 1, 1));
	REQUIRE(b4.getRect() == sf::FloatRect(4, 4, 1, 1));
	
	auto& b5 = physicsEngine.createStaticBodyAndGetTheReference({5, 5, 1, 1});
	REQUIRE(b1.getRect() == sf::FloatRect(1, 1, 1, 1));
	REQUIRE(b2.getRect() == sf::FloatRect(2, 2, 1, 1));
	REQUIRE(b3.getRect() == sf::FloatRect(3, 3, 1, 1));
	REQUIRE(b4.getRect() == sf::FloatRect(4, 4, 1, 1));
	REQUIRE(b5.getRect() == sf::FloatRect(5, 5, 1, 1));
}

TEST_CASE("kinematic bodies can be created and are accessible by returned reference")
{
	PhysicsEngine physicsEngine;

	auto& b1 = physicsEngine.createKinematicBodyAndGetTheReference({1, 1, 1, 1}, 1);
	REQUIRE(b1.getMass() == 1);

	auto& b2 = physicsEngine.createKinematicBodyAndGetTheReference({2, 2, 1, 1}, 2);
	REQUIRE(b1.getMass() == 1);
	REQUIRE(b2.getMass() == 2);

	auto& b3 = physicsEngine.createKinematicBodyAndGetTheReference({3, 3, 1, 1}, 3);
	REQUIRE(b1.getMass() == 1);
	REQUIRE(b2.getMass() == 2);
	REQUIRE(b3.getMass() == 3);

	auto& b4 = physicsEngine.createKinematicBodyAndGetTheReference({4, 4, 1, 1}, 4);
	REQUIRE(b1.getMass() == 1);
	REQUIRE(b2.getMass() == 2);
	REQUIRE(b3.getMass() == 3);
	REQUIRE(b4.getMass() == 4);

	auto& b5 = physicsEngine.createKinematicBodyAndGetTheReference({5, 5, 1, 1}, 5);
	REQUIRE(b1.getMass() == 1);
	REQUIRE(b2.getMass() == 2);
	REQUIRE(b3.getMass() == 3);
	REQUIRE(b4.getMass() == 4);
	REQUIRE(b5.getMass() == 5);
}

}