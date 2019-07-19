#include <catch.hpp>

#include "EntityComponentSystem/entity.hpp"
#include <memory>

namespace ph {

class Guy : public Entity
{
public:
	Guy() : Entity("guy"), mHp(100) {}
	void setHp(unsigned hp) { mHp = hp; }
	unsigned getHp() { return mHp; }

private:
	unsigned mHp;
};

TEST_CASE("Childs can be added to Entity object and are acessible.", "[EntityComponentSystem][Entity]")
{
	Entity root("root");
	root.addChild(std::make_unique<Entity>("kiddo"));
	auto& guy = std::make_unique<Guy>();
	guy->setHp(10);
	root.addChild(std::move(guy));
	CHECK_NOTHROW(root.getChild("kiddo"));
	auto& theSameGuy = dynamic_cast<Guy&>(root.getChild("guy"));
	CHECK(theSameGuy.getHp() == 10);
}

TEST_CASE("Exception is thrown when you try to get child which doesn't exist", "[EntityComponentSystem][Entity]")
{
	Entity root("root");
	CHECK_THROWS_WITH(root.getChild("rhinoceros"), "Child was not found!");
}

TEST_CASE("Right prefix is added to new child name when child of the same name already exists", "[EntityComponentSystem][Entity]")
{
	Entity root("root");
	for(int i = 0; i < 15; ++i)
		root.addChild(std::make_unique<Guy>());
	for(int i = 0; i < 15; ++i) {
		std::string name = i == 0 ? "guy" : "guy_" + std::to_string(i + 1);
		CHECK_NOTHROW(root.getChild(name));
	}
}

TEST_CASE("Child can be removed", "[EntityComponentSystem][Entity]")
{
	Entity root("root");
	root.addChild(std::make_unique<Guy>());
	root.addChild(std::make_unique<Entity>("Monkey"));
	auto& guy = root.getChild("guy");
	root.removeChild(&guy);
	CHECK_THROWS_WITH(root.getChild("guy"), "Child was not found!");
	CHECK_NOTHROW(root.getChild("Monkey"));
	root.removeChild("Monkey");
	CHECK_THROWS_WITH(root.getChild("Monkey"), "Child was not found!");
}

}