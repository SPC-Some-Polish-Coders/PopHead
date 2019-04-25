#include "Interface.hpp"
#ifndef GUI_BASE
#define GUI_BASE


namespace PopHead {
namespace GUI {

	class GUI
	{
	public:
		GUI() = default;
		~GUI();

		std::unique_ptr<Interface>& addInterface(const std::string& name);

		std::unique_ptr<Interface>& getInterface(const std::string& name);

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
		
		Base::GameData* mGameData;

		std::map<std::string, std::unique_ptr<Interface>> mInterfaceList;

	};

}}


#endif