#pragma once

#include "Scenes/gameObjectsParser.hpp"

namespace ph {

	class XmlGameObjectsParser : public GameObjectsParser
	{
		virtual void parseFile(const std::string& fileName) override;
	};
}
