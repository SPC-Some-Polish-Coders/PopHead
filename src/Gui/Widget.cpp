#include "Widget.hpp"
namespace PopHead {

namespace GUI {
	Widget::Widget()
		:
		mAlpha(0),
		mRoot(nullptr),
		mPosition(0, 0),
		mSize(0, 0),
		mOrigin(0.5, 0.5),
		mScale(1, 1),
		misActive(true),
		mGameData(nullptr),
		mWindow(nullptr)
	{
	}
	void Widget::draw()
	{
		if (misActive)
		{
			mWindow->draw(mSprite);
			/*for (const auto& k : mWidgetList)
			{
				k.second->draw();
			}*/
			for (auto k = mWidgetList.rbegin(); k != mWidgetList.rend(); k++)
			{
				if (k->second->isActive())
					k->second->draw();
			}
		}
	}

	void Widget::setAlpha(unsigned int alpha) 
	{
	}

	void Widget::update(sf::Time delta)
	{
		if (mGameData->getInput().getMouse().isMouseButtonJustPressed(sf::Mouse::Left))
		{

			auto k = mGameData->getInput().getMouse().getMousePosition();
			if (k.x > mSprite.getPosition().x && k.x < mSprite.getPosition().x + mSize.x &&
				k.y > mSprite.getPosition().y && k.y < mSprite.getPosition().y + mSize.y)
			{
				for (const auto& k : mBehaviors)
				{
					if (k.first == behaviorType::onPressed)
					{
						k.second(this);
					}
				}
			}
		}

		if (mGameData->getInput().getMouse().isMouseButtonJustReleased(sf::Mouse::Left))
		{
			for (const auto& k : mBehaviors)
			{
				if (k.first == behaviorType::onReleased)
				{
					k.second(this);
				}
			}
		}

		for (const auto& k : mBehaviors)
		{
			if (k.first == behaviorType::onUpdate)
			{
				k.second(this);
			}
		}
		

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
	//	auto k = mWidgetList.begin();
	//	k->second->setGameData(mGameData);
		//k->second->setRoot(this);
		ptr->setGameData(mGameData);
		ptr->setRoot(this);
		ptr->rePosition();
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
		//if (!mTexture.loadFromFile(path))
			//return false;
		mSprite.setTexture(mGameData->getTextures().get(path));
		mSize = mGameData->getTextures().get(path).getSize();

		mSize.x *= (unsigned int)mScale.x;
		mSize.y *= (unsigned int)mScale.y;
		mSprite.setScale(mScale);
		//rePosition();
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
			mSprite.setOrigin(mOrigin);
			
			mSprite.setPosition(pos.x * size.x + localPosition.x - mSize.x * mOrigin.x, pos.y * size.y + localPosition.y - mSize.y * mOrigin.y);

			for (const auto& k : mWidgetList)
			{
				k.second->rePosition();
			}
		}
	}

	void Widget::move(const sf::Vector2f& delta)
	{
		for (const auto& k : mWidgetList)
		{
			k.second->move(delta);
		}
		mSprite.move(delta);
	}

	void Widget::setScale(const sf::Vector2f& scale)
	{
		auto k = mGameData->getRenderer().getWindow().getSize();
		sf::Vector2f finalScale;
		finalScale.x = k.x / 1366.f;
		finalScale.y = k.y / 768.f;

		finalScale.x *= scale.x;
		finalScale.y *= scale.y;

		mSize.x *= (float)finalScale.x;
		mSize.y *= (float)finalScale.y;

		mSprite.setOrigin(mOrigin);
		mScale = finalScale;
		mSprite.scale(finalScale);
		setPosition(mPosition);

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
		mBehaviors.insert({ type,func });
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
		rePosition();
		for (const auto& k : mWidgetList)
		{
			k.second->rePosition();
		}
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
	void Widget::rePosition()
	{
		setPosition(mPosition);
		for (const auto& k : mWidgetList)
		{
			k.second->rePosition();
		}
	}
}}