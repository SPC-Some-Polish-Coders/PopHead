#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>

namespace ph {

// TODO_ren: Make Texture not store the size - size should be separate component

class Texture
{
public:
	Texture();
	Texture(const std::string& filepath);

	~Texture();

	bool loadFromFile(const std::string& filepath);
	void setData(void* rgbaData, unsigned arraySize, sf::Vector2i textureSize);

	void bind(unsigned slot = 0) const;

	sf::Vector2i getSize() const { return mSize; }
	int getWidth() const { return mSize.x; }
	int getHeight() const { return mSize.y; }

private:
	sf::Vector2i mSize;
	unsigned mID;
};

}
