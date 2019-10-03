#pragma once

#include <string>

namespace ph {

class Texture
{
public:
	Texture();

	void loadFromFile(const std::string&) const;

	void bind() const;

	static void setDefaultTextureSettings();

private:
	unsigned mID;
};

}
