#include "pch.hpp"
#include "widget.hpp"
#include "Renderer/renderer.hpp"
#include "Renderer/Shaders/embeddedShaders.hpp"

namespace ph {

static Shader guiShader;

void Widget::initShader()
{
	guiShader.init(shader::guiSrc());
}

void Widget::deleteShader()
{
	guiShader.remove();
}

Widget::Widget(const char* name)
	:mParent(Null)
	,mTexture(Null)
	,mLocalNormalizedPosition(0.f, 0.f)
	,mLocalNormalizedSize(0.f, 0.f)
	,mVelocity(0.f, 0.f)
	,mColor(sf::Color::White)
	,mIsActive(true)
{
	PH_ASSERT_UNEXPECTED_SITUATION(std::strlen(name) < 50, "Widget name length can be max 50 characters long!");
	std::strcpy(mName, name);
}

void Widget::handleEvent(sf::Event e)
{
	handleEventOnCurrent(e);
	handleEventOnChildren(e);
}

void Widget::handleEventOnCurrent(sf::Event e)
{
	if(e.type == sf::Event::MouseButtonPressed || e.type == sf::Event::MouseButtonReleased
		&& e.mouseButton.button == sf::Mouse::Left)
	{
		auto mousePos = Cast<Vec2>(sf::Mouse::getPosition(*sWindow));
		mousePos.x *= 1920.f / sScreenSize.x;
		mousePos.y *= 1080.f / sScreenSize.y;
		if(FloatRect(getScreenPosition(), getScreenSize()).contains(mousePos))
		{
			if(e.type == sf::Event::MouseButtonPressed) 
			{
				for(const auto& k : mBehaviors)
					if(k.first == BehaviorType::onPressed)
						k.second(this);
			}
			else if(e.type == sf::Event::MouseButtonReleased) 
			{
				for(const auto& k : mBehaviors)
					if(k.first == BehaviorType::onReleased)
						k.second(this);
			}
		}
	}
}

void Widget::handleEventOnChildren(sf::Event e)
{
	for(const auto& widget : mWidgetChildren)
		widget->handleEvent(e);
}

void Widget::update(float dt, u8 z)
{
	for(const auto& behaviour : mBehaviors)
		if(behaviour.first == BehaviorType::onUpdate)
			behaviour.second(this);

	move(mVelocity * dt);

	Renderer::submitQuad(mTexture, Null, &mColor, &guiShader,
		getScreenPosition(), getScreenSize(), z--, 0.f, {}, ProjectionType::gui, false);
	
	updateCurrent(dt, z - 2);
	updateChildren(dt, z);
}

void Widget::updateCurrent(float dt, u8 z)
{
}

void Widget::updateChildren(float dt, u8 z)
{
	for(const auto& widget : mWidgetChildren)
		widget->update(dt, z);
}

Widget* Widget::addChildWidget(Widget* ptr)
{
	ptr->setParent(this);
	return mWidgetChildren.emplace_back(ptr).get();
}

Widget* Widget::getWidget(const char* name)
{
	for(auto& widget : mWidgetChildren)
		if(std::strcmp(widget->getName(), name) == 0)
			return widget.get();
	return Null;
}

void Widget::addBehavior(BehaviorType type, const std::function<void(Widget*)>& func)
{
	mBehaviors.insert({type,func});
}

void Widget::hide()
{
	mIsActive = false;
}

void Widget::show()
{
	mIsActive = true;
}

void Widget::move(Vec2 offset)
{
	mLocalNormalizedPosition += offset;
}

void Widget::setCenterPosition(Vec2 centerPos)
{
	mLocalNormalizedPosition = centerPos - mLocalNormalizedSize / 2.f;
}

void Widget::setTopLeftPosition(Vec2 topLeftPos)
{
	mLocalNormalizedPosition = topLeftPos;
}

void Widget::setTopRightPosition(Vec2 topRightPos)
{
	mLocalNormalizedPosition = {topRightPos.x - mLocalNormalizedSize.x, topRightPos.y};
}

void Widget::setBottomLeftPosition(Vec2 bottomLeftPos)
{
	mLocalNormalizedPosition = {bottomLeftPos.x, bottomLeftPos.y - mLocalNormalizedSize.y};
}

void Widget::setBottomRightPosition(Vec2 bottomRightPos)
{
	mLocalNormalizedPosition = {bottomRightPos.x - mLocalNormalizedSize.x, bottomRightPos.y - mLocalNormalizedSize.y};
}

void Widget::setTopCenterPosition(Vec2 pos)
{
	mLocalNormalizedPosition = {pos.x - mLocalNormalizedSize.x / 2.f, pos.y};
}

void Widget::setBottomCenterPosition(Vec2 pos)
{
	mLocalNormalizedPosition = {pos.x - mLocalNormalizedSize.x / 2.f, pos.y - mLocalNormalizedSize.y};
}

void Widget::setRightCenterPosition(Vec2 pos)
{
	mLocalNormalizedPosition = {pos.x - mLocalNormalizedSize.x, pos.y - mLocalNormalizedSize.y / 2.f};
}

void Widget::setLeftCenterPosition(Vec2 pos)
{
	mLocalNormalizedPosition = {pos.x, pos.y - mLocalNormalizedSize.y / 2.f};
}

Vec2 Widget::getScreenPosition() const
{
	if(mParent) 
	{
		const auto parentPos = mParent->getScreenPosition(); 
		const auto parentSize = mParent->getScreenSize();
		return parentPos + Vec2(mLocalNormalizedPosition.x * parentSize.x, mLocalNormalizedPosition.y * parentSize.y);
	}
	else 
	{
		return Vec2(mLocalNormalizedPosition.x * 1920.f, mLocalNormalizedPosition.y * 1080.f);
	}
}

Vec2 Widget::getScreenSize() const
{
	if(mParent) 
	{
		const auto parentSize = mParent->getScreenSize();
		return Vec2(parentSize.x * mLocalNormalizedSize.x, parentSize.y * mLocalNormalizedSize.y);
	}
	else 
	{
		return Vec2(mLocalNormalizedSize.x * 1920.f, mLocalNormalizedSize.y * 1080.f);
	}
}

}

