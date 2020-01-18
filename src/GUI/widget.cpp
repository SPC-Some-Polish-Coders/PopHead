#include "widget.hpp"
#include "Renderer/renderer.hpp"
#include <cstring>

namespace ph {

Widget::Widget()
	:mParent(nullptr)
	,mTexture(nullptr)
	,mPosition(0, 0)
	,mSize(0, 0)
	,mIsActive(true)
{
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
		if(e->type == sf::Event::MouseButtonPressed || e->type == sf::Event::MouseButtonReleased
			&& e->mouseButton.button == sf::Mouse::Left)
		{
			auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*sWindow));
			if(FloatRect(mPosition, mSize).contains(mousePos))
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

void Widget::update(float dt)
{
	if(mIsActive) 
	{
		for(const auto& k : mBehaviors)
			if(k.first == BehaviorType::onUpdate)
				k.second(this);

		Renderer::submitQuad(mTexture, nullptr, &sf::Color(255, 255, 255, mAlpha), nullptr, mPosition, mSize, 10, 0.f, {}, ProjectionType::gui);

		for(const auto& widget : mChildren)
			widget->update(dt);
	}
}

void Widget::addChildWidget(const char* name, Widget* ptr)
{
	mChildren.emplace_back(ptr);
	ptr->setParent(this);
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

void Widget::setContentPath(const std::string& path)
{
	sTextures->load(path);
	mTexture = &sTextures->get(path);
	mSize = static_cast<sf::Vector2f>(mTexture->getSize());
	mDefaultSize = mSize;
	rePosition();
}

void Widget::setPosition(sf::Vector2f position)
{
	mLocalPosition = position;
	if(mParent != nullptr)
	{
		auto parentPos = mParent->getPosition();
		auto parentSize = mParent->getSize();
		mPosition = {parentPos.x * parentSize.x + mLocalPosition.x - mSize.x, parentPos.y * parentSize.y + mLocalPosition.y - mSize.y};

		for(const auto& widget : mChildren)
			widget->rePosition();
	}
}

void Widget::move(sf::Vector2f offset)
{
	mPosition += offset;
	for(const auto& widget : mChildren)
		widget->move(offset);
}

void Widget::scale(sf::Vector2f scale)
{
	mSize.x = unsigned int(scale.x * mSize.x);
	mSize.y = unsigned int(scale.y * mSize.y);
	this->scale(scale);
	rePosition();
	for(const auto& widget : mChildren)
		widget->scale(scale);
}

void Widget::addBehavior(BehaviorType type, const std::function<void(Widget*)>& func)
{
	mBehaviors.insert({type,func});
}

Widget* Widget::getWidget(const char* name)
{
	for(auto& widget : mChildren)
		if(std::strcmp(widget->getName(), name))
			return widget.get();
	return nullptr;
}

void Widget::rePosition()
{
	setPosition(mPosition);
	for(const auto& widget : mChildren)
		widget->rePosition();
}
	
}

