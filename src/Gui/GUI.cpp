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
			mInterfaceList.insert({ name,std::make_unique<Interface>(mGameData) });
			auto k = mInterfaceList.rbegin();
			k->second->setGameData(mGameData);
			return  k->second.get();
		}

		Widget* GUI::getInterface(const std::string& name)
		{
			return mInterfaceList.find(name)->second.get();
		}

		void GUI::move(const sf::Vector2f& delta)
		{ 
			for (const auto& k : mInterfaceList)
			{
				k.second->move(delta);
			}
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
			mGuiDrawer = new Gui_drawer(gamedata, "GUI_DRAWER", Renderer::LayerID::gui);
			mGuiDrawer->init(this);
			//mGameData->getRenderer().addObject(mGuiDrawer);


#ifdef GUI_TEST

			/*  GUI INTRODUCTION  

			

			auto interface = mGameData->getGui().addInterface("game_pause_menu");

		
			auto mainWidget = new Widget;
			auto buttonFirstWidget = new Widget;
			auto buttonSecondWidget = new Widget;
			auto thirdWidget = new Widget;
			buttonSecondWidget->setOrigin(sf::Vector2f(0.5, 0.5));
			//buttonSecondWidget->setScale(sf::Vector2f(1, 1));
			buttonSecondWidget->setPosition(sf::Vector2f(0.5, 0.7));
	
			interface->addWidget("buttonArea", mainWidget);
			mainWidget->addWidget("button1", buttonFirstWidget);
			mainWidget->addWidget("button2", buttonSecondWidget);
			

			interface->addWidget("area2", thirdWidget);

			
			

			
			mainWidget->setPosition(sf::Vector2f(0, 0)); 

			buttonFirstWidget->setOrigin(sf::Vector2f(0.5, 0.5));
			//buttonFirstWidget->setScale(sf::Vector2f(0.5, 0.5));
			buttonFirstWidget->setPosition(sf::Vector2f(0.5, 0.2));
			// interface->setContentPath("crazy_stuff.png");  --> nothing to do
			mGameData->getTextures().load("GuiTestContent/gui4.png");
			mGameData->getTextures().load("GuiTestContent/gui1.png");
			mGameData->getTextures().load("GuiTestContent/gui2.png");

			mainWidget->setContentPath("GuiTestContent/gui4.png");
			buttonFirstWidget->setContentPath("GuiTestContent/gui1.png");
			buttonSecondWidget->setContentPath("GuiTestContent/gui2.png");
			thirdWidget->setContentPath("GuiTestContent/gui2.png");


			
			interface->show();
			mainWidget->show();
			buttonFirstWidget->show();
			buttonSecondWidget->show();
			thirdWidget->show();
		

			
			interface->setPosition(sf::Vector2f(0, 0));



		
			mainWidget->setOrigin(sf::Vector2f(0.5, 0.5));


			//thirdWidget->setOrigin(sf::Vector2f(0, 0));
			thirdWidget->setPosition(sf::Vector2f(0.3, 0.3));
			
			buttonFirstWidget->addBehavior(behaviorType::onPressed, [](Widget * ptr)->void {
				ptr->setContentPath("src/Gui/GuiTestContent/gui2.png");
				});*/

#endif



		}
}}