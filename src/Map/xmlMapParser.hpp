#pragma once

#include "Scenes/mapParser.hpp"

namespace ph {

	class XmlMapParser : public MapParser
	{
		virtual void parseFile(const std::string& fileName) override;
	};
}
