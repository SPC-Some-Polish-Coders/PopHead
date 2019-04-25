#include "Interface.hpp"

namespace PopHead {

namespace GUI {
	Interface::Interface()
		: Widget()
	{
	}
	void Interface::update(sf::Time delta)
	{
		for (const auto& k : mWidgetList)
		{
			if(k.second->isActive())
				k.second->update(delta);
		}
	}

	void Interface::draw()
	{
		for (const auto& k : mWidgetList)
		{
			if (k.second->isActive())
				k.second->draw();
		}
	}

	bool Interface::setContentPath(const std::string& path)
	{
		/* Interface shouldn't be textured */
		return false;
	}

	void Interface::setPosition(const sf::Vector2f& pos)
	{
		auto k = mGameData->getRenderer().getWindow().getSize();
		mPosition.x = pos.x * k.x;
		mPosition.y = pos.y * k.y;
	}

	sf::Vector2f Interface::getGlobalPosition() const
	{
		return mPosition;
	}

}}


