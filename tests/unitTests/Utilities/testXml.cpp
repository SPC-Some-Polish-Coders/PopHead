#include <catch.hpp>
#include "Utilities/xml.hpp"

namespace ph {

namespace {
const std::string resFilepath = "tests/unitTests/res/";
}

TEST_CASE("Xml parsing", "[Utilities][Xml]")
{
	// test loadFromFile()
	Xml xmlDocument;
	CHECK_FALSE(xmlDocument.loadFromFile("asasd.xml"));
	REQUIRE(xmlDocument.loadFromFile(resFilepath + "test.xml"));

	// test getChild()
	auto root = xmlDocument.getChild("root");
	REQUIRE(root);
	auto pokemon = root->getChild("pokemon");
	CHECK_FALSE(pokemon);
	auto zombie = root->getChild("zombie");
	REQUIRE(zombie);

	// test getChildren()
	auto pokemons = root->getChildren("pokemon");
	CHECK(pokemons.empty());
	auto children = root->getChildren("child");
	REQUIRE(children.size() == 2);

	// test getChild(), getAttribute() and casting methods
	unsigned i = 0;
	for(auto& child : children)
	{
		auto body = child.getChild("body");
		REQUIRE(body);

		auto pos = body->getAttribute("position");
		REQUIRE(pos);
		auto z = body->getAttribute("z");
		REQUIRE(z);
		auto width = body->getAttribute("width");
		REQUIRE(width);
		auto height = body->getAttribute("height");
		REQUIRE(height);

		switch(i)
		{
		case 0: {
			CHECK(pos->toVector2f() == sf::Vector2f(12.4f, 1231.12f));
			CHECK(z->toUnsignedChar() == 111);
			CHECK(width->toChar() == 50);
			CHECK(height->toUnsigned() == 0);
		} break;

		case 1: {
			CHECK(pos->toVector2f() == sf::Vector2f(1.4f, -2.1f));
			CHECK(z->toFloat() == -20.f);
			CHECK(width->toInt() == 20);
			CHECK(height->toString() == "20");
			auto playerTag = child.getChild("playerTag");
			CHECK(playerTag);
		}
		default:
			break;
		}
		++i;
	}
	auto body = zombie->getChild("body");
	REQUIRE(body);
	auto info = zombie->getChild("info");
	REQUIRE(info);
	CHECK(info->getAttribute("isDangerous")->toBool() == true);
	CHECK(info->getAttribute("isFriendly")->toBool() == false);
	auto monsterTag = zombie->getChild("monsterTag");
	CHECK(monsterTag);
	auto playerTag = zombie->getChild("playerTag");
	CHECK_FALSE(playerTag);
}

}

