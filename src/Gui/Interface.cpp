#include "Interface.hpp"

namespace PopHead {

namespace GUI {

	void Interface::update(sf::Time delta)
	{
		for (const auto& k : mWidgetList)
		{
			if(k.second->isActive())
				k.second->update(delta);
		}
	}

	void Interface::draw()
	{
		for (const auto& k : mWidgetList)
		{
			if (k.second->isActive())
				k.second->draw();
		}
	}

	bool Interface::setContentPath(const std::string& path)
	{
		// Interface shouldn't be textured
		return false;
	}

}}


