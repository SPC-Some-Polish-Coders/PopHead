#include "catch.hpp"

#include "Utilities/random.hpp"

namespace ph {

TEST_CASE("is a generated number from given range", "[Utilities][Random]")
{
	for(int i = 0; i < 10; ++i)
	{ 
		DYNAMIC_SECTION("positive numbers" << i)
		{
			SECTION("small range")
			{
				float generatedPositiveNumber = Random::generateNumber(2.f, 4.f);
				CHECK(generatedPositiveNumber >= 2.f);
				CHECK(generatedPositiveNumber < 4.f);
			}
			SECTION("standard range")
			{
				float generatedPositiveNumber = Random::generateNumber(0.f, 20.f);
				CHECK(generatedPositiveNumber >= 0.f);
				CHECK(generatedPositiveNumber < 20.f);
			}
			SECTION("big range")
			{
				float generatedPositiveNumber = Random::generateNumber(1000.f, 20000.f);
				CHECK(generatedPositiveNumber >= 1000.f);
				CHECK(generatedPositiveNumber < 20000.f);
			}
			SECTION("limited range")
			{
				float generatedPositiveNumber = Random::generateNumber(1.f, 1.f);
				CHECK(generatedPositiveNumber == 1.f);
			}
		}
		DYNAMIC_SECTION("negative numbers" << i)
		{
			SECTION("small range")
			{
				float generatedNegativeNumber = Random::generateNumber(-4.f, -2.f);
				CHECK(generatedNegativeNumber >= -4.f);
				CHECK(generatedNegativeNumber < -2.f);
			}
			SECTION("standard range")
			{
				float generatedNegativeNumber = Random::generateNumber(-20.f, -10.f);
				CHECK(generatedNegativeNumber >= -20.f);
				CHECK(generatedNegativeNumber < -10.f);
			}
			SECTION("big range")
			{
				float generatedNegativeNumber = Random::generateNumber(-20.f, -10.f);
				CHECK(generatedNegativeNumber >= -20.f);
				CHECK(generatedNegativeNumber < -10.f);
			}
			SECTION("limited range")
			{
				float generatedNegativeNumber = Random::generateNumber(-1.f, -1.f);
				CHECK(generatedNegativeNumber == -1.f);
			}
		}
		DYNAMIC_SECTION("real numbers" << i)
		{
			SECTION("small range")
			{
				float generatedRealNumber = Random::generateNumber(-2.f, 2.f);
				CHECK(generatedRealNumber >= -2.f);
				CHECK(generatedRealNumber < 2.f);
			}
			SECTION("standard range")
			{
				float generatedRealNumber = Random::generateNumber(-20.f, 20.f);
				CHECK(generatedRealNumber >= -20.f);
				CHECK(generatedRealNumber < 20.f);
			}
			SECTION("big range")
			{
				float generatedRealNumber = Random::generateNumber(-2000.f, 2000.f);
				CHECK(generatedRealNumber >= -2000.f);
				CHECK(generatedRealNumber < 2000.f);
			}
		}
	}
}

}
