#include "Resources/TextureResource.hpp"

using PopHead::Resources::TextureResource;

bool TextureResource::load( std::string path )
{
    if (!mTexture.loadFromFile(path))
    {
        return false;
    }

    return true;
}
