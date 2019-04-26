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

			/*  GUI INTRODUCTION  */

			/*
				First we are going to create our Interface class (it can be: player equipment, skills menu, game options etc...).
			*/

			
			auto interface = mGameData->getGui().addInterface("game_pause_menu");

			/* Let's create some widgets to display. */
			auto mainWidget = new Widget;
			auto buttonFirstWidget = new Widget;
			auto buttonSecondWidget = new Widget;

			/* 
				Interface has no texture, it's only for positioning widgets and divide them into groups 
			*/

			// interface->setContentPath("crazy_stuff.png");  --> nothing to do
		

			mainWidget->setContentPath("src/Gui/GuiTestContent/gui4.png");
			buttonFirstWidget->setContentPath("src/Gui/GuiTestContent/gui1.png");
			buttonSecondWidget->setContentPath("src/Gui/GuiTestContent/gui2.png");


			/* By default, each gui element is disabled. This means that it will never be displayed.
			   You can manipulate displaying widgets by calling hide() or show().
			*/
			interface->show();
			mainWidget->show();
			buttonFirstWidget->show();
			buttonSecondWidget->show();

			/* each widget have to be connected (to widget or interface) */
			interface->addWidget("buttonArea", mainWidget);
			mainWidget->addWidget("button1", buttonFirstWidget);
			mainWidget->addWidget("button2", buttonSecondWidget);

			/* perfect center of the screen. */
			interface->setPosition(sf::Vector2f(0.5, 0.5));

			/* Order of method calls is important for correctly initializing widgets */ 
			mainWidget->setOrigin(sf::Vector2f(0.5, 0.5));
			mainWidget->setScale(sf::Vector2f(1, 1));  // ---> useless but important (dont ask)
			mainWidget->setPosition(sf::Vector2f(0, 0)); // ---> position oriented by parent-widget, so position (0,0) means left corner of the parent
		
			buttonFirstWidget->setOrigin(sf::Vector2f(0.5, 0.5));
			buttonFirstWidget->setScale(sf::Vector2f(0.5, 0.5));
			buttonFirstWidget->setPosition(sf::Vector2f(0.5, 0.2));
			
			buttonSecondWidget->setOrigin(sf::Vector2f(0.5, 0.5));
			buttonSecondWidget->setScale(sf::Vector2f(1, 1));
			buttonSecondWidget->setPosition(sf::Vector2f(0.5, 0.7));
			
			/* Let's set callback for our widget, for example change his texture after mouse button pressed */
			buttonFirstWidget->addBehavior(behaviorType::onPressed, [](Widget * ptr)->void {
				ptr->setContentPath("src/Gui/GuiTestContent/gui2.png");
				});


			/* 
				Other game window? 
				Just create new interface.
			*/
			


#endif



		}
}}