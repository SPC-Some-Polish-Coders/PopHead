#include "GUI.hpp"

namespace PopHead {
	namespace GUI {
		GUI::GUI()
			: mGuiDrawer(nullptr)
		{
		}
		GUI::~GUI()
		{
		}

		Widget* GUI::addInterface(const std::string& name)
		{
			mInterfaceList.insert({ name,std::make_unique<Interface>() });
			auto k = mInterfaceList.rbegin();
			k->second->setGameData(mGameData);
			return  k->second.get();
		}

		Widget* GUI::getInterface(const std::string& name)
		{
			return mInterfaceList.find(name)->second.get();
		}

		void GUI::deleteInterface(const std::string& name)
		{
			auto k = mInterfaceList.find(name);
			if (k != mInterfaceList.end())
			{
				mInterfaceList.erase(k);
			}
		}

		void GUI::showInterface(const std::string& name)
		{
			auto k = mInterfaceList.find(name);
			if (k != mInterfaceList.end())
			{
				k->second->show();
			}
		}

		void GUI::hideInterface(const std::string& name)
		{
			auto k = mInterfaceList.find(name);
			if (k != mInterfaceList.end())
			{
				k->second->hide();
			}
		}

		void GUI::swap(const std::string& first, const std::string& second)
		{
			auto Ifirst = mInterfaceList.find(first);
			auto Isecond = mInterfaceList.find(second);
			if (Ifirst != mInterfaceList.end() && Isecond != mInterfaceList.end())
			{
				std::swap(Ifirst, Isecond);
			}
		}

		void GUI::moveUp(const std::string& name)
		{
			if (!mInterfaceList.empty())
			{
				auto current = mInterfaceList.find(name);
				if (current != mInterfaceList.end())
				{
					auto top = mInterfaceList.end()--;
					std::swap(current, top);
				}
			}
		}

		void GUI::update(sf::Time deltaTime)
		{
			for (const auto& k : mInterfaceList)
			{
				if (k.second->isActive())
					k.second->update(deltaTime);
			}
		}

		void GUI::draw()
		{
			for (const auto& k : mInterfaceList)
			{
				if (k.second->isActive())
					k.second->draw();
			}
		}

		void GUI::transform()
		{

		}


		void GUI::init(Base::GameData* gamedata)
		{
			mGameData = gamedata;
			mGuiDrawer = new Gui_drawer(gamedata, "GUI_DRAWER", Renderer::LayerID::GUI);
			mGuiDrawer->init(this);
			mGameData->getRenderer().addObject(mGuiDrawer);


#ifdef GUI_TEST

/*			auto k = mGameData->getGui().addInterface("pause_game");
			k->setPosition(sf::Vector2f(0.5, 0.5));

			auto widget = new Widget;
			widget->setContentPath("src/Gui/GuiTestContent/gui4.png");
			k->addWidget("tlo_guziorow", widget);
			widget->setOrigin(sf::Vector2f(0.5, 0.5));
			widget->setPosition(sf::Vector2f(0, 0));
			widget->show();
			
			k->show();
			
			auto widget1 = new Widget;
			widget1->setContentPath("src/Gui/GuiTestContent/gui1.png");
			widget->addWidget("guzior1",widget1);
			widget1->setOrigin(sf::Vector2f(0.5, 0.5));
			widget1->setPosition(sf::Vector2f(0, 0));
			widget1->show();

			auto widget2 = new Widget;
			widget2->setContentPath("src/Gui/GuiTestContent/gui2.png");
			widget->addWidget("guzior2",widget2);
			widget2->setOrigin(sf::Vector2f(0.5, 0.5));
			widget2->setPosition(sf::Vector2f(0, 0));
			widget2->show();*/


#endif



		}
}}