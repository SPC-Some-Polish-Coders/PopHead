#pragma once

#include "Utilities/xml.hpp"

#include <string>
#include <vector>

namespace ph{

class SceneParser
{
public:
	SceneParser();

	std::vector<std::string> loadResources();
private:
	void loadTextures(std::vector<std::string>&, Xml&);
private:
	const std::string sourceName;

};

}
