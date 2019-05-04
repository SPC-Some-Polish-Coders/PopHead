#include "Base/game.hpp"

#include "Logs/Logs.hpp"

int main()
{
	PopHead::LOG(LogType::INFO, ModuleID::None, "start executing PopHead!");

	PopHead::Base::Game();
    return 0;
}
