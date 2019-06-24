#include "npc.hpp"

namespace ph {

Npc::Npc(GameData* gameData, std::string name)
	:Character(gameData, name)
{
}

void Npc::input()
{
    #if 0
    if(mGameData->getInput().getAction().isActionJustPressed("talk with NPC"))
        mWasDialogueButtonClicked = true;
    #endif // 0
}

void Npc::update(sf::Time delta)
{
    if(mWasDialogueButtonClicked)
        talk();
}

void Npc::talk()
{

}

}