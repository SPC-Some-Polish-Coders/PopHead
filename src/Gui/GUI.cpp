#include "GUI.hpp"

namespace PopHead {
	namespace GUI {

		GUI::~GUI()
		{
		}
		std::unique_ptr<Interface>& GUI::addInterface(const std::string& name)
		{
			return std::make_unique<Interface>();
		}
		std::unique_ptr<Interface>& GUI::getInterface(const std::string& name)
		{	
			return std::make_unique<Interface>();
		}

		void GUI::deleteInterface(const std::string& name)
		{
		}

		void GUI::showInterface(const std::string& name)
		{

		}

		void GUI::hideInterface(const std::string& name)
		{

		}

		void GUI::swap(const std::string& first, const std::string& second)
		{

		}

		void GUI::moveUp(const std::string& name)
		{

		}

		void GUI::update(sf::Time deltaTime)
		{

		}

		void GUI::draw()
		{

		}
		void GUI::transform()
		{
		}
	}
}