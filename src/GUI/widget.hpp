#pragma once

#include "widgetParent.hpp"
#include "behaviorType.hpp"
#include "Events/event.hpp"
#include "Renderer/API/texture.hpp"
#include "Resources/resourceHolder.hpp"
#include <map>
#include <functional>

namespace ph {

class Widget : public WidgetParent
{
public:
	Widget(const char* name);

	void handleEvent(const Event&);
	void update(float dt, unsigned char z);
private:
	virtual void handleEventOnCurrent(const ph::Event&);
	virtual void updateCurrent(float dt, unsigned char z);
	void handleEventOnChildren(const ph::Event&);
	void updateChildren(float dt, unsigned char z);

public:
	Widget* addChildWidget(Widget* ptr) override;
	void addBehavior(BehaviorType type, const std::function<void(Widget*)>& func);

	void hide();
	void show();

	void setParent(Widget* parent) { mParent = parent; };
	void setTexture(const Texture*);
	void setColor(sf::Color color) { mColor = color; }
	void setSize(sf::Vector2f size);
	void move(sf::Vector2f offset);

	void setCenterPosition(sf::Vector2f pos);
	void setTopLeftPosition(sf::Vector2f pos);
	void setTopRightPosition(sf::Vector2f pos);
	void setBottomLeftPosition(sf::Vector2f pos);
	void setBottomRightPosition(sf::Vector2f pos);
	void setTopCenterPosition(sf::Vector2f pos);
	void setBottomCenterPosition(sf::Vector2f pos);
	void setRightCenterPosition(sf::Vector2f pos);
	void setLeftCenterPosition(sf::Vector2f pos);

	const char* getName() { return mName; }
	bool isActive() { return mIsActive; }

	sf::Vector2f getLocalVirtualPosition() const { return mLocalNormalizedPosition; }
	sf::Vector2f getLocalVirtualSize() const { return mLocalNormalizedSize; }

	sf::Vector2f getScreenPosition() const;
	sf::Vector2f getScreenSize() const;

	static void setWindow(sf::Window* window) { sWindow = window; }
	static void setTextures(TextureHolder* textures) { sTextures = textures; }

protected:
	Widget* mParent;
	std::multimap<BehaviorType, std::function<void(Widget*)>> mBehaviors;

	char mName[50];

	const Texture* mTexture;
	sf::Vector2f mLocalNormalizedPosition;
	sf::Vector2f mLocalNormalizedSize;
	sf::Color mColor;
	bool mIsTextureSize;
	bool mIsActive;

	inline static sf::Window* sWindow;
	inline static TextureHolder* sTextures;
};

}

