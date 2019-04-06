#include "soundBufferResource.hpp"

using PopHead::Resources::SoundBufferResource;

bool SoundBufferResource::loadFromFile( std::string path )
{
    if(!mSoundBuffer.loadFromFile(path))
    {
        return false;
    }
    return true;
}
