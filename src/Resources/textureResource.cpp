#include "textureResource.hpp"

using PopHead::Resources::TextureResource;

bool TextureResource::loadFromFile( std::string path )
{
    if (!mTexture.loadFromFile(path))
    {
        return false;
    }
    return true;
}
