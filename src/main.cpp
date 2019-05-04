#include "Base/game.hpp"

#include "Logs/logManager.hpp"
#include "Logs/logType.hpp"

int main()
{
	auto& logManager = PopHead::Logs::LogManager::getLogManager();
	logManager.writeLog(PopHead::Logs::Log(PopHead::Logs::LogType::GOOD, PopHead::Logs::ModuleID::Logs, "start of executing PopHead!"));
	logManager.writeEachLog();

    PopHead::Base::Game();
    return 0;
}
