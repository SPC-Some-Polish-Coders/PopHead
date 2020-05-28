#pragma once

#include "behaviorType.hpp"
#include "Renderer/API/texture.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <map>
#include <functional>

namespace ph {

class Widget
{
public:
	Widget(const char* name);

	void handleEvent(sf::Event);
	void update(float dt, u8 z);
private:
	virtual void handleEventOnCurrent(sf::Event);
	virtual void updateCurrent(float dt, u8 z);
	void handleEventOnChildren(sf::Event);
	void updateChildren(float dt, u8 z);

public:
	Widget* addChildWidget(Widget* ptr);
	Widget* getWidget(const char* name);

	void addBehavior(BehaviorType type, const std::function<void(Widget*)>& func);

	void hide();
	void show();

	void setParent(Widget* parent) { mParent = parent; };
	void setTexture(Texture* texture) { mTexture = texture; }
	void setColor(sf::Color color) { mColor = color; }
	void setSize(Vec2 size) { mLocalNormalizedSize = size; }
	void setVelocity(Vec2 vel) { mVelocity = vel; }
	void move(Vec2 offset);

	void setCenterPosition(Vec2 pos);
	void setTopLeftPosition(Vec2 pos);
	void setTopRightPosition(Vec2 pos);
	void setBottomLeftPosition(Vec2 pos);
	void setBottomRightPosition(Vec2 pos);
	void setTopCenterPosition(Vec2 pos);
	void setBottomCenterPosition(Vec2 pos);
	void setRightCenterPosition(Vec2 pos);
	void setLeftCenterPosition(Vec2 pos);

	const char* getName() { return mName; }
	bool isActive() { return mIsActive; }

	Vec2 getLocalVirtualPosition() const { return mLocalNormalizedPosition; }
	Vec2 getLocalVirtualSize() const { return mLocalNormalizedSize; }

	Vec2 getScreenPosition() const;
	Vec2 getScreenSize() const;

	static void initShader();
	static void deleteShader();
	static void setWindow(sf::Window* window) { sWindow = window; }
	static void setScreenSize(Vec2 size) { sScreenSize = size; }

protected:
	char mName[50];

	std::multimap<BehaviorType, std::function<void(Widget*)>> mBehaviors;
	std::vector<std::unique_ptr<Widget>> mWidgetChildren;

	Widget* mParent;
	Texture* mTexture;
	Vec2 mLocalNormalizedPosition;
	Vec2 mLocalNormalizedSize;
	Vec2 mVelocity;
	sf::Color mColor;
	bool mIsActive;

	inline static sf::Window* sWindow;
	inline static Vec2 sScreenSize = Vec2(1920, 1080);
};

}

