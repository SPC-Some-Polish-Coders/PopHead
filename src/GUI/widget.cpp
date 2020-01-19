#include "widget.hpp"
#include "Renderer/renderer.hpp"
#include <cstring>

namespace ph {

Widget::Widget(const char* name)
	:mParent(nullptr)
	,mTexture(nullptr)
	,mLocalNormalizedPosition(0.f, 0.f)
	,mLocalNormalizedSize(0.f, 0.f)
	,mColor(sf::Color::White)
	,mIsTextureSize(false)
	,mIsActive(true)
{
	std::strcpy(mName, name);
}

void Widget::handleEvent(const ph::Event& phEvent)
{
	if(mIsActive) {
		handleEventOnCurrent(phEvent);
		handleEventOnChildren(phEvent);
	}
}

void Widget::handleEventOnCurrent(const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::MouseButtonPressed || e->type == sf::Event::MouseButtonReleased
			&& e->mouseButton.button == sf::Mouse::Left)
		{
			auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*sWindow));
			if(FloatRect(getScreenPosition(), getScreenSize()).contains(mousePos))
			{
				if(e->type == sf::Event::MouseButtonPressed) {
					for(const auto& k : mBehaviors)
						if(k.first == BehaviorType::onPressed)
							k.second(this);
				}
				else if(e->type == sf::Event::MouseButtonReleased) {
					for(const auto& k : mBehaviors)
						if(k.first == BehaviorType::onReleased)
							k.second(this);
				}
			}
		}
	}
}

void Widget::handleEventOnChildren(const ph::Event& phEvent)
{
	for(const auto& widget : mChildren)
		widget->handleEvent(phEvent);
}

void Widget::update(float dt, float z)
{
	if(mIsActive) 
	{
		for(const auto& behaviour : mBehaviors)
			if(behaviour.first == BehaviorType::onUpdate)
				behaviour.second(this);

		Renderer::submitQuad(mTexture, nullptr, &mColor, nullptr,
			getScreenPosition(), getScreenSize(), z--, 0.f, {}, ProjectionType::gui);

		updateCurrent(dt, z - 2);
		updateChildren(dt, z);
	}
}

void Widget::updateCurrent(float dt, float z)
{
}

void Widget::updateChildren(float dt, float z)
{
	for(const auto& widget : mChildren)
		widget->update(dt, z);
}

Widget* Widget::addChildWidget(Widget* ptr)
{
	ptr->setParent(this);
	return mChildren.emplace_back(ptr).get();
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

void Widget::setTexture(const std::string& path)
{
	sTextures->load(path);
	mTexture = &sTextures->get(path);
	auto textureSize = static_cast<sf::Vector2f>(mTexture->getSize());
	mLocalNormalizedSize = {textureSize.x / 1920.f, textureSize.y / 1080.f};
	mIsTextureSize = true;
}

void Widget::setSize(sf::Vector2f size)
{
	mIsTextureSize = false;
	mLocalNormalizedSize = size;
}

void Widget::move(sf::Vector2f offset)
{
	mLocalNormalizedPosition += offset;
}

void Widget::setCenterPosition(sf::Vector2f centerPos)
{
	mLocalNormalizedPosition = centerPos - mLocalNormalizedSize / 2.f;
}

void Widget::setTopLeftPosition(sf::Vector2f topLeftPos)
{
	mLocalNormalizedPosition = topLeftPos;
}

void Widget::setTopRightPosition(sf::Vector2f topRightPos)
{
	mLocalNormalizedPosition = {topRightPos.x - mLocalNormalizedSize.x, topRightPos.y};
}

void Widget::setBottomLeftPosition(sf::Vector2f bottomLeftPos)
{
	mLocalNormalizedPosition = {bottomLeftPos.x, bottomLeftPos.y - mLocalNormalizedSize.y};
}

void Widget::setBottomRightPosition(sf::Vector2f bottomRightPos)
{
	mLocalNormalizedPosition = {bottomRightPos.x - mLocalNormalizedSize.x, bottomRightPos.y - mLocalNormalizedSize.y};
}

void Widget::setTopCenterPosition(sf::Vector2f pos)
{
	mLocalNormalizedPosition = {pos.x - mLocalNormalizedSize.x / 2.f, pos.y};
}

void Widget::setBottomCenterPosition(sf::Vector2f pos)
{
	mLocalNormalizedPosition = {pos.x - mLocalNormalizedSize.x / 2.f, pos.y - mLocalNormalizedSize.y};
}

void Widget::setRightCenterPosition(sf::Vector2f pos)
{
	mLocalNormalizedPosition = {pos.x - mLocalNormalizedSize.x, pos.y - mLocalNormalizedSize.y / 2.f};
}

void Widget::setLeftCenterPosition(sf::Vector2f pos)
{
	mLocalNormalizedPosition = {pos.x, pos.y - mLocalNormalizedSize.y / 2.f};
}

Widget* Widget::getWidget(const char* name)
{
	for(auto& widget : mChildren)
		if(std::strcmp(widget->getName(), name))
			return widget.get();
	return nullptr;
}

sf::Vector2f Widget::getScreenPosition() const
{
	if(mParent) {
		const auto parentPos = mParent->getScreenPosition(); 
		const auto parentSize = mParent->getScreenSize();
		return parentPos + sf::Vector2f(mLocalNormalizedPosition.x * parentSize.x, mLocalNormalizedPosition.y * parentSize.y);
	}
	else {
		return sf::Vector2f(mLocalNormalizedPosition.x * 1920.f, mLocalNormalizedPosition.y * 1080.f);
	}
}

sf::Vector2f Widget::getScreenSize() const
{
	if(mIsTextureSize)
		return static_cast<sf::Vector2f>(mTexture->getSize());
	if(mParent) {
		const auto parentSize = mParent->getScreenSize();
		return sf::Vector2f(parentSize.x * mLocalNormalizedSize.x, parentSize.y * mLocalNormalizedSize.y);
	}
	else {
		return sf::Vector2f(mLocalNormalizedSize.x * 1920.f, mLocalNormalizedSize.y * 1080.f);
	}
}

}

