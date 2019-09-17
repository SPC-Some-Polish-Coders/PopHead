#include "widget.hpp"
#include "gameData.hpp"

namespace ph {

Widget::Widget()
	:mAlpha(0)
	,mRoot(nullptr)
	,mPosition(0, 0)
	,mSize(0, 0)
	,mOrigin(0, 0)
	,mScale(1, 1)
	,mIsActive(true)
	,mGameData(nullptr)
	,mWindow(nullptr)
{
}

void Widget::draw()
{
	if(mIsActive)
	{
		mWindow->draw(mSprite);
		for(auto k = mWidgetList.rbegin(); k != mWidgetList.rend(); k++)
			if(k->second->isActive())
				k->second->draw();
	}
}

void Widget::setAlpha(unsigned int alpha)
{
	mSprite.setColor(sf::Color(255, 255, 255, alpha));
}

void Widget::handleEvent(const ph::Event& phEvent)
{
	if(!mIsActive)
		return;
	
	handleEventOnCurrent(phEvent);
	handleEventOnChildren(phEvent);
}

void Widget::handleEventOnCurrent(const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::MouseButtonReleased && e->mouseButton.button == sf::Mouse::Left)
		{
			auto c = sf::Mouse::getPosition(mGameData->getRenderWindow());
			auto k = mWindow->mapPixelToCoords(c);

			// TODO: Maybe use Rect::doPositiveRectsIntersect()
			if(k.x > mSprite.getPosition().x && k.x < mSprite.getPosition().x + mSize.x &&
				k.y > mSprite.getPosition().y && k.y < mSprite.getPosition().y + mSize.y)
			{
				for(const auto& k : mBehaviors)
					if(k.first == BehaviorType::onReleased)
						k.second(this);
			}
		}
	}
}

void Widget::handleEventOnChildren(const ph::Event& phEvent)
{
	for(const auto& widget : mWidgetList)
		widget.second->handleEvent(phEvent);
}

void Widget::update(sf::Time delta)
{
	if(mIsActive == false)
		return;

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		auto c = sf::Mouse::getPosition(mGameData->getRenderWindow());
		auto k = mWindow->mapPixelToCoords(c);

		if(k.x > mSprite.getPosition().x && k.x < mSprite.getPosition().x + mSize.x &&
			k.y > mSprite.getPosition().y && k.y < mSprite.getPosition().y + mSize.y)
		{
			for(const auto& k : mBehaviors)
			{
				if(k.first == BehaviorType::onPressed)
				{
					k.second(this);
				}
			}
		}
	}

	for(const auto& k : mBehaviors)
		if(k.first == BehaviorType::onUpdate)
			k.second(this);

	for(const auto& k : mWidgetList)
		k.second->update(delta);
}

void Widget::addWidget(const std::string& name, Widget* ptr)
{
	mWidgetList.insert({name,std::unique_ptr<Widget>(ptr)});
	ptr->setGameData(mGameData);
	ptr->setRoot(this);
	ptr->rePosition();
}

void Widget::hide()
{
	mIsActive = false;
}

void Widget::show()
{
	mIsActive = true;
}

bool Widget::setContentPath(const std::string& path)
{
	mSprite.setTexture(mGameData->getTextures().get(path));
	mSize = mGameData->getTextures().get(path).getSize();

	mDefaultSize = mSize;
	scale(mScale);
	rePosition();
	return true;
}

void Widget::setPosition(const sf::Vector2f& pos)
{
	mPosition = pos;
	if(mRoot != nullptr)
	{
		auto localPosition = mRoot->getGlobalPosition();
		auto size = mRoot->getSize();
		mSprite.setOrigin(mOrigin);

		mSprite.setPosition(pos.x * size.x + localPosition.x - mSize.x * mOrigin.x, pos.y * size.y + localPosition.y - mSize.y * mOrigin.y);

		for(const auto& k : mWidgetList)
			k.second->rePosition();
	}
}

void Widget::move(const sf::Vector2f& delta)
{
	mSprite.move(delta);
}

void Widget::moveAlongBranch(const sf::Vector2f& delta)
{
	this->move(delta);
	for(const auto& k : mWidgetList)
		k.second->moveAlongBranch(delta);
}

void Widget::scale(const sf::Vector2f& scale)
{
	mSize.x = unsigned int(scale.x * mSize.x);
	mSize.y = unsigned int(scale.y * mSize.y);
	mSprite.setOrigin(mOrigin);
	mSprite.scale(scale);
	rePosition();
}

void Widget::scaleAlongBranch(const sf::Vector2f& scale)
{
	this->scale(scale);
	rePosition();
	for(const auto& k : mWidgetList)
		k.second->scaleAlongBranch(scale);
}

void Widget::setAlphaAlongBranch(unsigned int alpha)
{
	this->setAlpha(alpha);
	for(const auto& k : mWidgetList)
		k.second->setAlphaAlongBranch(alpha);
}

void Widget::setVirtualSize(const sf::Vector2f& size)
{
}

sf::Vector2u Widget::getSize() const
{
	return mSize;
}

void Widget::addBehavior(BehaviorType type, const std::function<void(Widget*)>& func)
{
	mBehaviors.insert({type,func});
}

Widget* Widget::getWidget(const std::string& name)
{
	auto it = mWidgetList.find(name);

	if(it != mWidgetList.end())
		return it->second.get();

	return nullptr;
}

void Widget::transform(const sf::Vector2f pos, const sf::Vector2f size)
{
}

void Widget::setGameData(GameData* GameData)
{
	mGameData = GameData;
	mWindow = dynamic_cast<sf::RenderWindow*>(&GameData->getRenderWindow());
}

bool Widget::isActive()
{
	return mIsActive;
}

void Widget::setRoot(Widget* ptr)
{
	mRoot = ptr;
}

void Widget::setOrigin(const sf::Vector2f& origin)
{
	mOrigin = origin;
	rePosition();
	for(const auto& k : mWidgetList)
		k.second->rePosition();
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
	for(const auto& k : mWidgetList)
		k.second->rePosition();
}

}