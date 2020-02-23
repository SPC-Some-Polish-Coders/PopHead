#pragma once

#include <string>

namespace ph {

class Texture;

bool loadTexture(const std::string& filePath, bool srgb = true);
Texture& getTexture(const std::string& filePath); 

}

