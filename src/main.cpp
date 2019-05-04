#include "Base/game.hpp"

#include "Logs.hpp"

int main()
{
	PopHead::LOG(LogType::GOOD, ModuleID::None, "start executing PopHead!");

    PopHead::Base::Game();
    return 0;
}
