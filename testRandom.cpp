#include "catch.hpp"

#include "Utilities/random.hpp"

namespace ph {

TEST_CASE("is a generated number from given range", "[Utilities][Random]")
{
	SECTION("positive numbers")
	{
		SECTION("small range")
		{
			float generatedPositiveNumber = Random::generateNumber(2.f, 4.f);
			REQUIRE(generatedPositiveNumber >= 2.f);
			REQUIRE(generatedPositiveNumber < 4.f);
		}
		SECTION("standard range")
		{
			float generatedPositiveNumber = Random::generateNumber(0.f, 20.f);
			REQUIRE(generatedPositiveNumber >= 0.f);
			REQUIRE(generatedPositiveNumber < 20.f);
		}
		SECTION("big range")
		{
			float generatedPositiveNumber = Random::generateNumber(1000.f, 20000.f);
			REQUIRE(generatedPositiveNumber >= 1000.f);
			REQUIRE(generatedPositiveNumber < 20000.f);
		}
		SECTION("limited range")
		{
			float generatedPositiveNumber = Random::generateNumber(1.f, 1.f);
			REQUIRE(generatedPositiveNumber == 1.f);
		}
	}
	SECTION("negative numbers")
	{
		SECTION("small range")
		{
			float generatedNegativeNumber = Random::generateNumber(-4.f, -2.f);
			REQUIRE(generatedNegativeNumber >= -4.f);
			REQUIRE(generatedNegativeNumber < -2.f);
		}
		SECTION("standard range")
		{
			float generatedNegativeNumber = Random::generateNumber(-20.f, -10.f);
			REQUIRE(generatedNegativeNumber >= -20.f);
			REQUIRE(generatedNegativeNumber < -10.f);
		}
		SECTION("big range")
		{
			float generatedNegativeNumber = Random::generateNumber(-20.f, -10.f);
			REQUIRE(generatedNegativeNumber >= -20.f);
			REQUIRE(generatedNegativeNumber < -10.f);
		}
		SECTION("limited range")
		{
			float generatedNegativeNumber = Random::generateNumber(-1.f, -1.f);
			REQUIRE(generatedNegativeNumber == -1.f);
		}
	}
	SECTION("real numbers")
	{
		SECTION("small range")
		{
			float generatedRealNumber = Random::generateNumber(-2.f, 2.f);
			REQUIRE(generatedRealNumber >= -2.f);
			REQUIRE(generatedRealNumber < 2.f);
		}
		SECTION("standard range")
		{
			float generatedRealNumber = Random::generateNumber(-20.f, 20.f);
			REQUIRE(generatedRealNumber >= -20.f);
			REQUIRE(generatedRealNumber < 20.f);
		}
		SECTION("big range")
		{
			float generatedRealNumber = Random::generateNumber(-2000.f, 2000.f);
			REQUIRE(generatedRealNumber >= -2000.f);
			REQUIRE(generatedRealNumber < 2000.f);
		}
	}
}

}
