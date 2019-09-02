#include <catch.hpp>

#include "Resources/resourceHolder.hpp"
#include "../TestsUtilities/bufferedHandler.hpp"

namespace ph {

	TEST_CASE("Cannot load non-existing texture", "[Resources][TextureHolder][ResourceHolder]")
	{
		TextureHolder textures;
		CHECK_FALSE(textures.load("dsfgkjnsejrgkjnkjdefnkjenf"));   // make sure that this file is not real
	}

	TEST_CASE("Cannot get non-existing texture", "[Resources][TextureHolder][ResourceHolder]")
	{
		Tests::BufferedHandler logs;
		logs.clearRecords();

		TextureHolder textures;
		CHECK_THROWS(textures.get("kjnaefjnshgnkjsdbgfhesbrgvjhnejv"));  // make sure that this file is not real
		CHECK(logs.getRecordsCount() == 1);
	}
}