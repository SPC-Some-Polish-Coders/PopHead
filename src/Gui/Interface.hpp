
#include "Widget.hpp"

#ifndef GUI_INTERFACE
#define GUI_INTERFACE

namespace PopHead {

namespace GUI {


	class Interface 
		:public Widget
	{
	public:

		Interface();

		void update(sf::Time delta);

		void draw();

		bool setContentPath(const std::string& path);

		void setPosition(const sf::Vector2f& pos);

		void addWidget(const std::string& name, Widget* ptr);

		sf::Vector2f getGlobalPosition() const;
	};

}}


#endif