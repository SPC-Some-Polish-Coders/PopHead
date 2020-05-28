#include "pch.hpp"
#include "texture.hpp"
#include "openglErrors.hpp"

//#define STB_IMAGE_IMPLEMENTATION - uncomment if we don't link to sfml-graphics module
#include <stb_image.h>

namespace ph {

Texture::Texture()
{
	GLCheck( glGenTextures(1, &mID) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, mID) );

	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
}

Texture::Texture(u32 existingID, Vec2i size)
	:mSize(size)
	,mID(existingID)
{
}

Texture::~Texture()
{
	GLCheck( glDeleteTextures(1, &mID) );
}

bool Texture::loadFromFile(const std::string& filepath, bool srgb)
{
	stbi_set_flip_vertically_on_load(true);
	i32 numberOfChanels;
	u8* data = stbi_load(filepath.c_str(), &mSize.x, &mSize.y, &numberOfChanels, 0);
	stbi_set_flip_vertically_on_load(false);

	if(data == Null)
		return false;

	GLenum dataFormat = 0, internalDataFormat = 0;
	if(numberOfChanels == 3) 
	{
		internalDataFormat = srgb ? GL_SRGB8 : GL_RGB8;
		dataFormat = GL_RGB;
		GLCheck( glPixelStorei(GL_UNPACK_ALIGNMENT, 1) );
	}
	else if(numberOfChanels == 4) 
	{
		internalDataFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
		dataFormat = GL_RGBA;
		GLCheck( glPixelStorei(GL_UNPACK_ALIGNMENT, 4) );
	}
	else
	{
		PH_EXIT_GAME("Texture format of \"" + filepath + "\" is unsupported!");
	}

	GLCheck( glTexImage2D(GL_TEXTURE_2D, 0, internalDataFormat, mSize.x, mSize.y, 0, dataFormat, GL_UNSIGNED_BYTE, data) );
	GLCheck( glGenerateMipmap(GL_TEXTURE_2D) );

	stbi_image_free(data);

	return true;
}

void Texture::setData(void* rgbaData, u32 arraySize, Vec2i textureSize)
{
	// TODO_ren: Make possible setting data for different formats rgb, rgba (now it's only rgba)
	
	PH_ASSERT_CRITICAL(arraySize == 4 * textureSize.x * textureSize.y, "Data must be for entire texture!");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);
}

void Texture::bind(u32 slot) const
{
	GLCheck( glActiveTexture(GL_TEXTURE0 + slot) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, mID) );
}

}

