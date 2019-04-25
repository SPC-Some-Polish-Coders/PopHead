#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <memory>

#include "BehaviorType.hpp"

#ifndef GUI_BASE_WIDGET
#define GUI_BASE_WIDGET

namespace PopHead {

namespace Base { class GameData; }

namespace GUI {


	class Widget {
	public:

		virtual void draw();

		virtual void setAlpha(unsigned int alpha);

		virtual void update(sf::Time delta);

		virtual void addWidget(const std::unique_ptr<Widget>& ptr);

		virtual void hide();

		virtual void show();

		virtual bool setContentPath(const std::string& path);

		virtual void setPosition(const sf::Vector2f& pos);

		virtual void setScale(const sf::Vector2f& scale);

		virtual void setVirtualSize(const sf::Vector2f& size);

		virtual void addBehavior(behaviorType type, const std::function<void(Widget*)>& func);

		virtual Widget* getWidget(const std::string& name);

		virtual void transform(const sf::Vector2f pos, const sf::Vector2f size);

		void setGameData(Base::GameData* GameData);

	protected:
		sf::Vector2f mPosition;
		sf::Sprite mSprite;
		bool misActive;
		sf::Vector2i mSize;
		sf::Vector2i mVirtualSize;
		unsigned int mAlpha;
		Base::GameData* mGameData;
		
		std::multimap < behaviorType, std::function<void(Widget*)>> mBehaviors;
		std::multimap<std::string, std::unique_ptr<Widget>> mWidgetList;
	};




}}





#endif