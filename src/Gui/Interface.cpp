#include "Interface.hpp"
#include "gameData.hpp"

namespace ph {

	Interface::Interface()
	{
		mSize = sf::Vector2u(300, 300);
		mDefaultSize = mSize;
		scale(sf::Vector2f(1, 1));
	}
	Interface::Interface(GameData* data)
		: Widget()
	{
		mGameData = data;

		mWindow = dynamic_cast<sf::RenderWindow*>(&mGameData->getRenderer().getWindow());

		//mSize = sf::Vector2u(300, 300);
		mSize = mWindow->getSize();
		mDefaultSize = mSize;
		scale(sf::Vector2f(1, 1));

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
		if(isActive())
		for (auto k = mWidgetList.rbegin(); k != mWidgetList.rend(); k++)
		{
			if (k->second->isActive())
				k->second->draw();
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
		mPosition.x = pos.x * k.x /2;
		mPosition.y = pos.y * k.y /2;
		for (const auto& k : mWidgetList)
		{
			k.second->rePosition();
		}
	}

	void Interface::addWidget(const std::string& name, Widget* ptr)
	{
		Widget::addWidget(name,ptr);
	}

	void Interface::move(const sf::Vector2f& delta)
	{
	}

	sf::Vector2f Interface::getGlobalPosition() const
	{
		return mPosition;
	}

}
