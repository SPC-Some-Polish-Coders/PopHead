#pragma once

#include "Scenes/guiParser.hpp"

namespace ph {

	class XmlGuiParser : public GUIParser
	{
		virtual void parseFile(const std::string& fileName) override;
	};
}
