
#include "TextWidget.hpp"

#ifndef GUI_INTERFACE
#define GUI_INTERFACE

namespace ph {

namespace GUI {


	class Interface 
		:public Widget
	{
	public:
		Interface();

		Interface(GameData* data);

		void update(sf::Time delta);

		void draw();

		bool setContentPath(const std::string& path);

		void setPosition(const sf::Vector2f& pos);

		void addWidget(const std::string& name, Widget* ptr);

		void move(const sf::Vector2f& delta);

		sf::Vector2f getGlobalPosition() const;
	};

}}


#endif