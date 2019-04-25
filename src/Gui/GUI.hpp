#include "Interface.hpp"

#include "World/Entity/object.hpp"
#define GUI_TEST

#ifndef GUI_BASE
#define GUI_BASE





namespace PopHead {
namespace GUI {

	class GUI
	{
	public:
		GUI();
		~GUI();

		class Gui_drawer
			: public World::Entity::Object
		{
		public:

			Gui_drawer(Base::GameData* gameData, std::string name, Renderer::LayerID id)
				: World::Entity::Object(gameData, name, id)
			{
			}


			void init(GUI* gui) {
				mGui = gui;
			}
			void onCollision(Object&) {};

			void draw(sf::RenderTarget& target, sf::RenderStates states) const override
			{
				mGui->draw();
			}

		private:
			GUI* mGui;
		};


		Widget* addInterface(const std::string& name);

		Widget* getInterface(const std::string& name);

		void deleteInterface(const std::string& name);

		void showInterface(const std::string& name);

		void hideInterface(const std::string& name);

		void swap(const std::string& first, const std::string& second);

		void moveUp(const std::string& name);

		void update(sf::Time deltaTime);

		void transform();

		void draw();

		void init(Base::GameData* gamedata);

	private:
		
		Gui_drawer* mGuiDrawer;

		Base::GameData* mGameData;

		std::map<std::string, std::unique_ptr<Interface>> mInterfaceList;

	};

}}


#endif