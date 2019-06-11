#ifndef POPHEAD_TERMINAL_COMMANDS_COMMAND_H_
#define POPHEAD_TERMINAL_COMMANDS_COMMAND_H_

#include <string>

namespace PopHead {
namespace Terminal {

class Command
{
public:
	Command(const std::string& command);

private:
	std::string mCommand;
};

}}

#endif // !POPHEAD_TERMINAL_COMMANDS_COMMAND_H_

