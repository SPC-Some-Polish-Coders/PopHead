#include "texture.hpp"
#include "openglErrors.hpp"
#include <stdexcept>
#include <GL/glew.h>

//#define STB_IMAGE_IMPLEMENTATION - uncomment if we don't link to sfml-graphics module
#include <stb_image.h>

namespace ph {

Texture::Texture()
{
	GLCheck( glGenTextures(1, &mID) );
	GLCheck( glBindTexture(GL_TEXTURE_2D, mID) );
}

void Texture::loadFromFile(const std::string& filepath) const
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, numberOfChanels;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &numberOfChanels, 0);
	if(data) {
		GLCheck( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data) );
		GLCheck( glGenerateMipmap(GL_TEXTURE_2D) );
	}
	else
		throw std::runtime_error("Texture wasn't correctly loaded from file: " + filepath);

	stbi_image_free(data);
}

void Texture::bind() const
{
	GLCheck( glBindTexture(GL_TEXTURE_2D, mID) );
}

void Texture::setDefaultTextureSettings()
{
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
	GLCheck( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
}

}
