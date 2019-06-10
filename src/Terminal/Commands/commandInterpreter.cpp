#include "commandInterpreter.hpp"

#include "Utilities/debug.hpp"

using PopHead::Terminal::CommandInterpreter;

void CommandInterpreter::handleCommand(const std::string& command)
{
	if(command == "test")
		PH_LOG(LogType::Info, "terminal test");
}
