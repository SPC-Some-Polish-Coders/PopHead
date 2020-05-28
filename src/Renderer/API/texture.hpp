#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>

namespace ph {

class Texture
{
public:
	Texture();
	Texture(u32 existingID, Vec2i size);

	~Texture();

	bool loadFromFile(const std::string& filepath, bool srgb);
	void setData(void* rgbaData, u32 arraySize, Vec2i textureSize);

	void bind(u32 slot = 0) const;

	Vec2i getSize() const { return mSize; }
	i32 getWidth() const { return mSize.x; }
	i32 getHeight() const { return mSize.y; }
	u32 getID() const { return mID; }

private:
	Vec2i mSize;
	u32 mID;
};

}

