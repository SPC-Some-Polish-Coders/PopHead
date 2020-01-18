#pragma once

#include "behaviorType.hpp"
#include "Events/event.hpp"
#include "Renderer/API/texture.hpp"
#include "Resources/resourceHolder.hpp"
#include <map>
#include <vector>
#include <functional>
#include <memory>

namespace ph {

class Widget 
{
public:
	Widget();

	void handleEvent(const Event&);
	virtual void update(float dt);

	void addChildWidget(const char* name, Widget* ptr);
	void addBehavior(BehaviorType type, const std::function<void(Widget*)>& func);

	void hide();
	void show();

	void setParent(Widget* parent) { mParent = parent; };
	void setAlpha(unsigned int alpha) { mAlpha = alpha; }
	void setContentPath(const std::string& path);
	void setPosition(sf::Vector2f pos);
	void rePosition();
	void move(sf::Vector2f offset);
	void scale(sf::Vector2f scale);

	const char* getName() { return mName; }
	Widget* getWidget(const char* name);
	sf::Vector2f getPosition() { return mPosition; }
	sf::Vector2f getSize() { return mSize; } 
	bool isActive() { return mIsActive; }

	static void setWindow(sf::Window* window) { sWindow = window; }
	static void setTextures(TextureHolder* textures) { sTextures = textures; }

private:
	virtual void handleEventOnCurrent(const ph::Event&);
	virtual void handleEventOnChildren(const ph::Event&);

protected:
	Widget* mParent;
	std::vector<std::unique_ptr<Widget>> mChildren;
	std::multimap<BehaviorType, std::function<void(Widget*)>> mBehaviors;

	char mName[50];

	Texture* mTexture;
	sf::Vector2f mPosition;
	sf::Vector2f mLocalPosition;
	sf::Vector2f mSize;
	sf::Vector2f mVirtualSize;
	sf::Vector2f mDefaultSize;
	unsigned char mAlpha; // TODO_gui: Alpha handling or color multiplication
	bool mIsActive;

	inline static sf::Window* sWindow;
	inline static TextureHolder* sTextures;
};

}

