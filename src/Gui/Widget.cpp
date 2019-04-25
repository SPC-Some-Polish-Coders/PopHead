#include "Widget.hpp"
namespace PopHead {

namespace GUI {

	auto Widget::draw() -> void
	{
	}

	auto Widget::setAlpha(unsigned int alpha) -> void
	{
	}

	auto Widget::update(sf::Time delta) -> void
	{
	}

	auto Widget::addWidget(const std::unique_ptr<Widget>& ptr) -> void
	{
	}

	auto Widget::hide() -> void
	{
	}

	auto Widget::show() -> void
	{
	}

	auto Widget::setContentPath(const std::string& path) -> bool
	{
		return false;
	}

	auto Widget::setPosition(const sf::Vector2f& pos) -> void
	{
	}

	auto Widget::setScale(const sf::Vector2f& scale) -> void
	{
	}

	auto Widget::setVirtualSize(const sf::Vector2f& size) -> void
	{
	}

	auto Widget::addBehavior(behaviorType type, const std::function<void(Widget*)>& func) -> void
	{
	}
	auto Widget::getWidget(const std::string& name) -> Widget*
	{
		return nullptr;
	}
	auto Widget::transform(const sf::Vector2f pos, const sf::Vector2f size) -> void
	{
	}
	void Widget::setGameData(Base::GameData* GameData)
	{
		mGameData = GameData;
	}
}}