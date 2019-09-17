#pragma once

#include "behaviorType.hpp"
#include "Events/event.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <memory>

namespace ph {

class GameData;

class Widget 
{
public:
	Widget();

	virtual void draw();

	virtual void setAlpha(unsigned int alpha);

	void handleEvent(const ph::Event&);

	virtual void update(sf::Time delta);

	virtual void addWidget(const std::string& name, Widget* ptr);

	virtual void hide();

	virtual void show();

	virtual bool setContentPath(const std::string& path);

	virtual void setPosition(const sf::Vector2f& pos);

	virtual void move(const sf::Vector2f& delta);

	virtual void moveAlongBranch(const sf::Vector2f& delta);

	virtual void scale(const sf::Vector2f& scale);

	virtual void scaleAlongBranch(const sf::Vector2f& scale);

	virtual void setAlphaAlongBranch(unsigned int alpha);

	virtual sf::Vector2u getSize() const;

	virtual void addBehavior(BehaviorType type, const std::function<void(Widget*)>& func);

	virtual Widget* getWidget(const std::string& name);

	virtual void setGameData(GameData* GameData);

	virtual bool isActive();

	virtual void setRoot(Widget* ptr);

	virtual void setOrigin(const sf::Vector2f& origin);
	virtual sf::Vector2f getOrigin() const;

	virtual sf::Vector2f getPosition() const;
	virtual sf::Vector2f getGlobalPosition() const;

	virtual void rePosition();

private:
	virtual void handleEventOnCurrent(const ph::Event&);
	virtual void handleEventOnChildren(const ph::Event&);
	virtual void transform(const sf::Vector2f pos, const sf::Vector2f size);
	void setVirtualSize(const sf::Vector2f& size);

protected:
	std::multimap<std::string, std::unique_ptr<Widget>> mWidgetList;
	std::multimap < BehaviorType, std::function<void(Widget*)>> mBehaviors;
	GameData* mGameData;
	sf::RenderWindow* mWindow;
	sf::Vector2f mPosition;
	sf::Vector2u mSize;
	sf::Vector2i mVirtualSize;
	sf::Vector2f mOrigin;
	sf::Vector2f mScale;
	sf::Vector2u mDefaultSize;
	unsigned int mAlpha;
	bool mIsActive;

private:
	sf::Sprite mSprite;
	Widget* mRoot;
};


}
