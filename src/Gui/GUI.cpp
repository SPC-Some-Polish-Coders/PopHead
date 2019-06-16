#include "GUI.hpp"

namespace ph {


		


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
				k.second->moveAlongBranch(delta);
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


		void GUI::init(GameData* gamedata)
		{
			mGameData = gamedata;
			mGuiDrawer = new Gui_drawer(gamedata, "GUI_DRAWER", LayerID::gui);
			mGuiDrawer->init(this);
			//mGameData->getRenderer().addObject(mGuiDrawer);


#ifdef GUI_TEST

			/* some wild examples */

			mGameData->getTextures().load("GuiTestContent/gui4.png");
			mGameData->getTextures().load("GuiTestContent/gui1.png");
			mGameData->getTextures().load("GuiTestContent/gui2.png");
			mGameData->getFonts().load("GuiTestContent/testFont.ttf");

			auto interface = mGameData->getGui().addInterface("game_pause_menu");

			auto mainWidget = new Widget;
			auto buttonFirstWidget = new TextWidget;
			auto buttonSecondWidget = new Widget;
			interface->addWidget("buttonArea", mainWidget);
			mainWidget->addWidget("button1", buttonFirstWidget);
			mainWidget->addWidget("button2", buttonSecondWidget);
		//	auto mainSecondWidget = new Widget;
			//buttonSecondWidget->setOrigin(sf::Vector2f(0.5, 0.5));
			//buttonSecondWidget->setScale(sf::Vector2f(1, 1));
			//buttonSecondWidget->setPosition(sf::Vector2f(0.5, 0.7));

			//mainWidget->setPosition(sf::Vector2f(0.5, 0.5));
			mainWidget->setContentPath("GuiTestContent/gui4.png");
			mainWidget->setOrigin(sf::Vector2f(0.5, 0.5));
			//mainWidget->setPosition(sf::Vector2f(1, 1));
			//mainWidget->setScale(sf::Vector2f(3, 8));
			//interface->setPosition(sf::Vector2f(0.3, 0.3));
			//interface->addWidget("area2", mainSecondWidget);
			//mainWidget->addWidget("kek", mainSecondWidget);
			//mainSecondWidget->setContentPath("GuiTestContent/gui2.png");
			//mainSecondWidget->setOrigin(sf::Vector2f(0.5, 0.5));
			//mainSecondWidget->setPosition(sf::Vector2f(1, 1));

			buttonFirstWidget->setContentPath("GuiTestContent/gui1.png");
			buttonFirstWidget->setOrigin(sf::Vector2f(0.5f, 0.5f));
			buttonFirstWidget->setPosition(sf::Vector2f(0.5f, 0.3f));

			buttonFirstWidget->setFontPath("GuiTestContent/testFont.ttf");
			buttonFirstWidget->setTextPosition(sf::Vector2f(0.5f, 0.5f));
			buttonFirstWidget->setString("klik");
			buttonFirstWidget->setColor(sf::Color::Green);
			buttonFirstWidget->setTextPosition(sf::Vector2f(0.4f, 0.3f));

			buttonFirstWidget->addBehavior(behaviorType::onPressed, [](Widget* ptr)->void
			{
					auto widget = dynamic_cast<TextWidget*>(ptr);
					widget->setString("kliknieto");
					widget->setTextPosition(sf::Vector2f(0.3f, 0.3f));
			});
			
			buttonSecondWidget->setContentPath("GuiTestContent/gui2.png");
			buttonSecondWidget->setOrigin(sf::Vector2f(0.5f, 0.5f));
			buttonSecondWidget->setPosition(sf::Vector2f(0.5f, 0.7f));
		
			
			//interface->moveAlongBranch(sf::Vector2f(-50, -110));

			
#endif



		}
}