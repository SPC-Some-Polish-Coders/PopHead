
#include "Widget.hpp"

#ifndef GUI_INTERFACE
#define GUI_INTERFACE

namespace PopHead {

namespace GUI {


	class Interface 
		:public Widget
	{
	public:

		void update(sf::Time delta);

		void draw();

		bool setContentPath(const std::string& path);
	};

}}


#endif