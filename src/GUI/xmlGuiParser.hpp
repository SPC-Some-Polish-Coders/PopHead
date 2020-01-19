#pragma once

#include "Resources/resourceHolder.hpp"
#include <string>

namespace ph {

class GUI;

void parseGuiXml(const std::string& fileName, GUI*, TextureHolder*);

}
