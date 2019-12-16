#include "gui.hpp"
#include "gameData.hpp"

namespace ph {

GUI::Gui_drawer::Gui_drawer(GameData* gameData, std::string name)
	:mGui(nullptr)
{

}

void GUI::Gui_drawer::init(GUI* gui) {
	mGui = gui;
}

void GUI::Gui_drawer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	mGui->draw();
}

void GUI::Gui_drawer::update(sf::Time delta)
{
	mGui->update(delta);
}

void GUI::Gui_drawer::move(sf::Vector2f delta, bool recursive)
{
	mGui->move(delta);
}

GUI::GUI()
	:mGuiDrawer(nullptr)
	,mGameData(nullptr)
{
}

GUI::~GUI()
{
}

Widget* GUI::addInterface(const std::string& name)
{
	auto iter = mInterfaceList.insert({name,std::make_unique<Interface>(mGameData)});
	if (iter.second)
	{
		iter.first->second->setGameData(mGameData);
		return iter.first->second.get();
	}
	return nullptr;
}

Widget* GUI::getInterface(const std::string& name)
{
	return mInterfaceList.find(name)->second.get();
}

void GUI::move(const sf::Vector2f& delta)
{
	for(const auto& k : mInterfaceList)
		k.second->moveAlongBranch(delta);
}

void GUI::deleteInterface(const std::string& name)
{
	auto k = mInterfaceList.find(name);
	if(k != mInterfaceList.end())
		mInterfaceList.erase(k);
}

void GUI::showInterface(const std::string& name)
{
	auto k = mInterfaceList.find(name);
	if(k != mInterfaceList.end())
		k->second->show();
}

void GUI::hideInterface(const std::string& name)
{
	auto k = mInterfaceList.find(name);
	if(k != mInterfaceList.end())
		k->second->hide();
}

void GUI::swap(const std::string& first, const std::string& second)
{
	auto Ifirst = mInterfaceList.find(first);
	auto Isecond = mInterfaceList.find(second);
	if(Ifirst != mInterfaceList.end() && Isecond != mInterfaceList.end())
		std::swap(Ifirst, Isecond);
}

void GUI::moveUp(const std::string& name)
{
	if(!mInterfaceList.empty())
	{
		auto current = mInterfaceList.find(name);
		if(current != mInterfaceList.end()) {
			auto top = mInterfaceList.end()--;
			std::swap(current, top);
		}
	}
}

void GUI::handleEvent(const ph::Event& e)
{
	for(const auto& i : mInterfaceList)
		i.second->handleEvent(e);
}

void GUI::update(sf::Time dt)
{
	for(const auto& k : mInterfaceList) {
		if(k.second->isActive())
			k.second->update(dt);
	}
}

void GUI::draw()
{
	for(const auto& k : mInterfaceList)
		if(k.second->isActive())
			k.second->draw();
}

void GUI::transform()
{
}

void GUI::init(GameData* gamedata)
{
	mGameData = gamedata;
}

void GUI::clearGUI()
{
	mGuiDrawer.reset(new Gui_drawer(mGameData, "GUI_DRAWER"));
	mGuiDrawer->init(this);
	mInterfaceList.clear();
}

}