#include "texture.hpp"
#include "openglErrors.hpp"
#include "Logs/logs.hpp"
#include <stdexcept>
#include <GL/glew.h>

//#define STB_IMAGE_IMPLEMENTATION - uncomment if we don't link to sfml-graphics module
#include <stb_image.h>

namespace ph {

Texture::Texture()
{
	GLCheck( glGenTextures(1, &mID) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, mID) );

	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
}

Texture::Texture(const std::string& filepath)
	:Texture()
{
	loadFromFile(filepath);
}

Texture::~Texture()
{
	GLCheck( glDeleteTextures(1, &mID) );
}

bool Texture::loadFromFile(const std::string& filepath)
{
	stbi_set_flip_vertically_on_load(true);
	int numberOfChanels;
	unsigned char* data = stbi_load(filepath.c_str(), &mSize.x, &mSize.y, &numberOfChanels, 0);

	if(data == nullptr)
		return false;

	GLenum dataFormat = 0, internalDataFormat = 0;
	if(numberOfChanels == 3) {
		internalDataFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}
	else if(numberOfChanels == 4) {
		internalDataFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else
		PH_EXIT_GAME("Texture format of \"" + filepath + "\" is unsupported!");

	GLCheck( glTexImage2D(GL_TEXTURE_2D, 0, internalDataFormat, mSize.x, mSize.y, 0, dataFormat, GL_UNSIGNED_BYTE, data) );
	GLCheck( glGenerateMipmap(GL_TEXTURE_2D) );

	stbi_image_free(data);

	return true;
}

void Texture::bind(unsigned slot) const
{
	GLCheck( glActiveTexture(GL_TEXTURE0 + slot) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, mID) );
}

}
