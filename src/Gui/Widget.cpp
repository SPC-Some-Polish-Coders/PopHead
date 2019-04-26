#include "Widget.hpp"
namespace PopHead {

namespace GUI {
	Widget::Widget()
		:
		mRoot(nullptr),
		mPosition(0, 0),
		mSize(0,0),
		mOrigin(0.5,0.5)
	{
		//mSprite.setPosition(sf::Vector2f(500, 500));

	}
	void Widget::draw()
	{
		if (misActive)
		{
			mWindow->draw(mSprite);
			for (const auto& k : mWidgetList)
			{
				k.second->draw();
			}
		}
	}

	void Widget::setAlpha(unsigned int alpha) 
	{
	}

	void Widget::update(sf::Time delta)
	{
		if (misActive)
		{
			for (const auto& k : mWidgetList)
			{
				k.second->update(delta);
			}
		}
	}

	void Widget::addWidget(const std::string& name, Widget* ptr) 
	{
		mWidgetList.insert({name,std::unique_ptr<Widget>(ptr) });
		auto k = mWidgetList.rbegin();
		k->second->setGameData(mGameData);
		k->second->setRoot(this);
	}

	void Widget::hide() 
	{
		misActive = false;
	}

	void Widget::show()
	{
		misActive = true;
	}

	bool Widget::setContentPath(const std::string& path)
	{
		if (!mTexture.loadFromFile(path))
			return false;
		mSize = mTexture.getSize();
		mSprite.setTexture(mTexture);
		mSize = mTexture.getSize();
		return true;
	}

	void Widget::setPosition(const sf::Vector2f& pos) 
	{
		mPosition = pos;
		if (mRoot != nullptr)
		{
			auto localPosition = mRoot->getGlobalPosition();
			auto size = mRoot->getSize();
			auto origin = mRoot->getOrigin();

			//mSprite.setPosition(pos.x * size.x + localPosition.x - mSize.x * mOrigin.x, pos.y * size.y + localPosition.y - mSize.y * mOrigin.y);

			mSprite.setPosition(pos.x * size.x + localPosition.x + origin.x * size.x - mSize.x * mOrigin.x, pos.y * size.y + localPosition.y + origin.y * size.y - mSize.y * mOrigin.y);
		}
	}

	void Widget::setScale(const sf::Vector2f& scale)
	{
		mSize.x *= scale.x;
		mSize.y *= scale.y;
		mSprite.scale(scale);
	}

	void Widget::setVirtualSize(const sf::Vector2f& size) 
	{
	}

	sf::Vector2u Widget::getSize() const
	{
		return mSize;
	}

	void Widget::addBehavior(behaviorType type, const std::function<void(Widget*)>& func) 
	{
	}
	Widget* Widget::getWidget(const std::string& name)
	{
		return nullptr;
	}
	void Widget::transform(const sf::Vector2f pos, const sf::Vector2f size) 
	{
	}
	void Widget::setGameData(Base::GameData* GameData)
	{
		mGameData = GameData;
		mWindow = dynamic_cast<sf::RenderWindow*>(&GameData->getRenderer().getWindow());
	}
	bool Widget::isActive()
	{
		return misActive;
	}
	void Widget::setRoot(Widget* ptr)
	{
		mRoot = ptr;
	}
	void Widget::setOrigin(const sf::Vector2f& origin)
	{
		mOrigin = origin;
	}
	sf::Vector2f Widget::getOrigin() const
	{
		return mOrigin;
	}
	sf::Vector2f Widget::getPosition() const
	{
		return mPosition;
	}
	sf::Vector2f Widget::getGlobalPosition() const
	{
		return mSprite.getPosition();
	}
}}