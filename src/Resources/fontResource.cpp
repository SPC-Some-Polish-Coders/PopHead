#include "fontResource.hpp"

using PopHead::Resources::FontResource;

bool FontResource::loadFromFile( std::string path )
{
    if(!mFont.loadFromFile(path))
    {
        return false;
    }
    return true;
}
