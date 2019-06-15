#include "npc.hpp"

ph::Npc::Npc(GameData* gameData, std::string name)
	:Character(gameData, name)
{
}

void ph::Npc::input()
{
    #if 0
    if(mGameData->getInput().getAction().isActionJustPressed("talk with NPC"))
        mWasDialogueButtonClicked = true;
    #endif // 0
}

void ph::Npc::update(sf::Time delta)
{
    if(mWasDialogueButtonClicked)
        talk();
}

void ph::Npc::talk()
{

}
