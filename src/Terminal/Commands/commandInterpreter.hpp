#ifndef POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_
#define POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_

#include <string>

namespace PopHead {
namespace Terminal {

class CommandInterpreter
{
public:
	void handleCommand(const std::string&);
};

}}

#endif // !POPHEAD_TERMINAL_COMMANDS_COMMANDINTERPRETER_H_

