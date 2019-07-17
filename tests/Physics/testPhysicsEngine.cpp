#include <catch.hpp>

#include "Physics/physicsEngine.hpp"
#include <vector>

namespace ph {

TEST_CASE("static bodies can be created and are accessible by returned reference", "[Physics][PhysicsEngine]")
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

TEST_CASE("kinematic bodies can be created and are accessible by returned reference", "[Physics][PhysicsEngine]")
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

TEST_CASE("static bodies can be removed from physics engine", "[Physics][PhysicsEngine]")
{
	PhysicsEngine physicsEngine;

	auto& b1 = physicsEngine.createStaticBodyAndGetTheReference({0, 0, 5, 5});
	auto& b2 = physicsEngine.createStaticBodyAndGetTheReference({7, 7, 3, 3});

	CHECK_NOTHROW(physicsEngine.removeStaticBody(b1));
	CHECK(physicsEngine.howManyStaticBodiesAreThere() == 1);
	CHECK(b2.getRect() == sf::FloatRect(7, 7, 3, 3));
}

TEST_CASE("kinematic bodies can be removed from physics engine", "[Physics][PhysicsEngine]")
{
	PhysicsEngine physicsEngine;

	auto& b1 = physicsEngine.createKinematicBodyAndGetTheReference({0, 0, 1, 1}, 1);
	auto& b2 = physicsEngine.createKinematicBodyAndGetTheReference({2, 2, 3, 3}, 2);

	CHECK_NOTHROW(physicsEngine.removeKinematicBody(b1));
	CHECK(physicsEngine.howManyKinematicBodiesAreThere() == 1);
	CHECK(b2.getMass() == 2);
}

TEST_CASE("Kinematic body moves diagonally wiping two static bodies", "[Physics][PhysicsEngine]")
{
	PhysicsEngine physicsEngine;
	const sf::Time deltaTime = sf::seconds(1.f / 60.f);

	SECTION("wall is on the left of player") {
		physicsEngine.createStaticBodyAndGetTheReference({-30, 0, 30, 10});
		physicsEngine.createStaticBodyAndGetTheReference({-30, 10, 30, 10});
		
		SECTION("player moves left down") {
			auto& player = physicsEngine.createKinematicBodyAndGetTheReference({0, 5, 10, 10}, 25);
			player.move({-10, 10});
			physicsEngine.update(deltaTime);
			CHECK(player.getPosition().x == 0);
			CHECK(player.getPosition().y == 15);
		}
		SECTION("player moves left up") {
			auto& player = physicsEngine.createKinematicBodyAndGetTheReference({0, 15, 10, 10}, 25);
			player.move({-10, -10});
			physicsEngine.update(deltaTime);
			CHECK(player.getPosition().x == 0);
			CHECK(player.getPosition().y == 5); // this assertion catches the error
		}
	}

	SECTION("wall is on the right of player") {
		physicsEngine.createStaticBodyAndGetTheReference({10, 0, 30, 10});
		physicsEngine.createStaticBodyAndGetTheReference({10, 10, 30, 10});
		
		SECTION("player moves right down") {
			auto& player = physicsEngine.createKinematicBodyAndGetTheReference({0, 5, 10, 10}, 25);
			player.move({10, 10});
			physicsEngine.update(deltaTime);
			CHECK(player.getPosition().x == 0);
			CHECK(player.getPosition().y == 15);
		}
		SECTION("player moves right up") {
			auto& player = physicsEngine.createKinematicBodyAndGetTheReference({0, 15, 10, 10}, 25);
			player.move({10, -10});
			physicsEngine.update(deltaTime);
			CHECK(player.getPosition().x == 0);
			CHECK(player.getPosition().y == 5); // this assertion catches the error
		}
	}
	
	SECTION("wall is above the player") {
		physicsEngine.createStaticBodyAndGetTheReference({0, -30, 10, 30});
		physicsEngine.createStaticBodyAndGetTheReference({10, -30, 10, 30});

		SECTION("player moves right up") {
			auto& player = physicsEngine.createKinematicBodyAndGetTheReference({5, 0, 10, 10}, 25);
			player.move({10, -10});
			physicsEngine.update(deltaTime);
			CHECK(player.getPosition().x == 15);
			CHECK(player.getPosition().y == 0);
		}
		SECTION("player moves left up") {
			auto& player = physicsEngine.createKinematicBodyAndGetTheReference({15, 0, 10, 10}, 25);
			player.move({-10, -10});
			physicsEngine.update(deltaTime);
			CHECK(player.getPosition().x == 5);
			CHECK(player.getPosition().y == 0);
		}
	}
	
	SECTION("wall is beneath the player") {
		physicsEngine.createStaticBodyAndGetTheReference({0, 10, 10, 30});
		physicsEngine.createStaticBodyAndGetTheReference({10, 10, 10, 30});

		SECTION("player moves right down") {
			auto& player = physicsEngine.createKinematicBodyAndGetTheReference({5, 0, 10, 10}, 25);
			player.move({10, 10});
			physicsEngine.update(deltaTime);
			CHECK(player.getPosition().x == 15);
			CHECK(player.getPosition().y == 0);
		}
		SECTION("player moves left down") {
			auto& player = physicsEngine.createKinematicBodyAndGetTheReference({15, 0, 10, 10}, 25);
			player.move({-10, 10});
			physicsEngine.update(deltaTime);
			CHECK(player.getPosition().x == 5);
			CHECK(player.getPosition().y == 0);
		}
	}
}

}