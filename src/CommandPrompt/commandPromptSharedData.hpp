#ifndef POPHEAD_COMMANDPROMPT_COMMANDPROMPTSHAREDDATA_H_
#define POPHEAD_COMMANDPROMPT_COMMANDPROMPTSHAREDDATA_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

namespace PopHead {
namespace CommandPrompt {

struct CommandPromptData
{
	sf::Text mText;
	std::string mContent;
	bool mIsVisible = false;
};

using CommandPromptSharedData = std::shared_ptr<CommandPromptData>;

}}

#endif // !POPHEAD_COMMANDPROMPT_COMMANDPROMPTSHAREDDATA_H_

