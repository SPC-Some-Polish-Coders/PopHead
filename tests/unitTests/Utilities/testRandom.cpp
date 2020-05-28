#include "catch.hpp"

#include "Utilities/random.hpp"

namespace ph {

TEST_CASE("Generated number is from given range", "[Utilities][Random]")
{
	for(i32 i = 0; i < 10; ++i)
	{ 
		DYNAMIC_SECTION("Positive numbers" << i)
		{
			SECTION("floats")
			{
				SECTION("Small range")
				{
					float generatedPositiveNumber = Random::generateNumber(2.f, 4.f);
					CHECK(generatedPositiveNumber >= 2.f);
					CHECK(generatedPositiveNumber <= 4.f);
				}
				SECTION("Standard range")
				{
					float generatedPositiveNumber = Random::generateNumber(0.f, 20.f);
					CHECK(generatedPositiveNumber >= 0.f);
					CHECK(generatedPositiveNumber <= 20.f);
				}
				SECTION("Big range")
				{
					float generatedPositiveNumber = Random::generateNumber(1000.f, 20000.f);
					CHECK(generatedPositiveNumber >= 1000.f);
					CHECK(generatedPositiveNumber <= 20000.f);
				}
				SECTION("Limited range")
				{
					float generatedPositiveNumber = Random::generateNumber(1.f, 1.f);
					CHECK(generatedPositiveNumber == 1.f);
				}
			}
			SECTION("i32egers") 
			{
				SECTION("Small range")
				{
					i32 generatedPositiveNumber = Random::generateNumber(2, 4);
					CHECK(generatedPositiveNumber >= 2);
					CHECK(generatedPositiveNumber <= 4);
				}
				SECTION("Standard range")
				{
					i32 generatedPositiveNumber = Random::generateNumber(0, 20);
					CHECK(generatedPositiveNumber >= 0);
					CHECK(generatedPositiveNumber <= 20);
				}
				SECTION("Big range")
				{
					i32 generatedPositiveNumber = Random::generateNumber(1000, 20000);
					CHECK(generatedPositiveNumber >= 1000);
					CHECK(generatedPositiveNumber <= 20000);
				}
				SECTION("Limited range")
				{
					i32 generatedPositiveNumber = Random::generateNumber(1, 1);
					CHECK(generatedPositiveNumber == 1);
				}
			}
		}
		DYNAMIC_SECTION("Negative numbers" << i)
		{
			SECTION("floats") 
			{
				SECTION("Small range")
				{
					float generatedNegativeNumber = Random::generateNumber(-4.f, -2.f);
					CHECK(generatedNegativeNumber >= -4.f);
					CHECK(generatedNegativeNumber <= -2.f);
				}
				SECTION("Standard range")
				{
					float generatedNegativeNumber = Random::generateNumber(-20.f, -10.f);
					CHECK(generatedNegativeNumber >= -20.f);
					CHECK(generatedNegativeNumber <= -10.f);
				}
				SECTION("Big range")
				{
					float generatedNegativeNumber = Random::generateNumber(-2000.f, -1000.f);
					CHECK(generatedNegativeNumber >= -2000.f);
					CHECK(generatedNegativeNumber <= -1000.f);
				}
				SECTION("Limited range")
				{
					float generatedNegativeNumber = Random::generateNumber(-1.f, -1.f);
					CHECK(generatedNegativeNumber == -1.f);
				}
			}
			SECTION("i32egers") 
			{
				SECTION("Small range")
				{
					i32 generatedNegativeNumber = Random::generateNumber(-4, -2);
					CHECK(generatedNegativeNumber >= -4);
					CHECK(generatedNegativeNumber <= -2);
				}
				SECTION("Standard range")
				{
					i32 generatedNegativeNumber = Random::generateNumber(-20, -10);
					CHECK(generatedNegativeNumber >= -20);
					CHECK(generatedNegativeNumber <= -10);
				}
				SECTION("Big range")
				{
					i32 generatedNegativeNumber = Random::generateNumber(-2000, -1000);
					CHECK(generatedNegativeNumber >= -2000);
					CHECK(generatedNegativeNumber <= -1000);
				}
				SECTION("Limited range")
				{
					i32 generatedNegativeNumber = Random::generateNumber(-1, -1);
					CHECK(generatedNegativeNumber == -1);
				}
			}
		}
		DYNAMIC_SECTION("Real numbers" << i)
		{
			SECTION("floats")
			{
				SECTION("Small range")
				{
					float generatedRealNumber = Random::generateNumber(-2.f, 2.f);
					CHECK(generatedRealNumber >= -2.f);
					CHECK(generatedRealNumber <= 2.f);
				}
				SECTION("Standard range")
				{
					float generatedRealNumber = Random::generateNumber(-20.f, 20.f);
					CHECK(generatedRealNumber >= -20.f);
					CHECK(generatedRealNumber <= 20.f);
				}
				SECTION("Big range")
				{
					float generatedRealNumber = Random::generateNumber(-2000.f, 2000.f);
					CHECK(generatedRealNumber >= -2000.f);
					CHECK(generatedRealNumber <= 2000.f);
				}
			}
			SECTION("i32egers")
			{
				SECTION("Small range")
				{
					i32 generatedRealNumber = Random::generateNumber(-2, 2);
					CHECK(generatedRealNumber >= -2);
					CHECK(generatedRealNumber <= 2);
				}
				SECTION("Standard range")
				{
					i32 generatedRealNumber = Random::generateNumber(-20, 20);
					CHECK(generatedRealNumber >= -20);
					CHECK(generatedRealNumber <= 20);
				}
				SECTION("Big range")
				{
					i32 generatedRealNumber = Random::generateNumber(-2000, 2000);
					CHECK(generatedRealNumber >= -2000);
					CHECK(generatedRealNumber <= 2000);
				}
			}
		}
	}
}

}
