#include "npc.hpp"

using ph::World::Entity::NPC;


NPC::NPC(ph::Base::GameData* gameData, std::string name)
:Character(gameData, name)
{
}

void NPC::input()
{
    #if 0
    if(mGameData->getInput().getAction().isActionJustPressed("talk with NPC"))
        wasDialogueButtonClicked = true;
    #endif // 0
}

void NPC::update(sf::Time delta)
{
    if(wasDialogueButtonClicked)
        talk();
}

void NPC::talk()
{

}
