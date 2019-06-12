#include "Interface.hpp"

namespace PopHead {

namespace GUI {
	Interface::Interface()
	{
		mSize = sf::Vector2u(300, 300);
		setScale(sf::Vector2f(1, 1));
	}
	Interface::Interface(Base::GameData* data)
		: Widget()
	{
		mGameData = data;

		mWindow = dynamic_cast<sf::RenderWindow*>(&mGameData->getRenderer().getWindow());

		mSize = sf::Vector2u(300, 300);
		setScale(sf::Vector2f(1, 1));

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
		/*for (const auto& k : mWidgetList)
		{
			if (k.second->isActive())
				k.second->draw();
		}*/
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
		mPosition.x = pos.x * k.x;
		mPosition.y = pos.y * k.y;
		for (const auto& k : mWidgetList)
		{
			k.second->rePosition();
		}
	}

	void Interface::addWidget(const std::string& name, Widget* ptr)
	{
		Widget::addWidget(name,ptr);
		//mSize = ptr->getSize();
	}

	void Interface::move(const sf::Vector2f& delta)
	{
		for (const auto& k : mWidgetList)
		{
			k.second->move(delta);
		}
	}

	sf::Vector2f Interface::getGlobalPosition() const
	{
		return mPosition;
	}

}}


