#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <memory>

#include "BehaviorType.hpp"
#include "World/Entity/object.hpp"
#include "Base/gameData.hpp"
#ifndef GUI_BASE_WIDGET
#define GUI_BASE_WIDGET

namespace ph {

class GameData; 

namespace GUI {


	class Widget {
	public:

		Widget();

		virtual void draw();

		virtual void setAlpha(unsigned int alpha);

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

		virtual void addBehavior(behaviorType type, const std::function<void(Widget*)>& func);

		virtual Widget* getWidget(const std::string& name);


		virtual void setGameData(GameData* GameData);

		virtual bool isActive();

		virtual void setRoot( Widget* ptr);

		virtual void setOrigin(const sf::Vector2f& origin);
		virtual sf::Vector2f getOrigin() const;
		
		virtual sf::Vector2f getPosition() const;
		virtual sf::Vector2f getGlobalPosition() const;

		virtual void rePosition();
	protected:
		sf::Vector2f mPosition;
		bool misActive;
		sf::Vector2u mSize;
		sf::Vector2i mVirtualSize;
		sf::Vector2f mOrigin;
		unsigned int mAlpha;
		sf::Vector2f mScale;
		GameData* mGameData;
		sf::RenderWindow* mWindow;
		std::multimap < behaviorType, std::function<void(Widget*)>> mBehaviors;
		std::multimap<std::string, std::unique_ptr<Widget>> mWidgetList;
		sf::Vector2u mDefaultSize;
	private:
		sf::Sprite mSprite;
		sf::Texture mTexture;
		Widget* mRoot;
		
		
		
		virtual void transform(const sf::Vector2f pos, const sf::Vector2f size);
		void setVirtualSize(const sf::Vector2f& size);

	};


}}

#endif