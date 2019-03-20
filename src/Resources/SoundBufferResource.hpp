#ifndef POPHEAD_RESOURCE_SOUNDBUFFERRESOURCE_H_
#define POPHEAD_RESOURCE_SOUNDBUFFERRESOURCE_H_

#include <string>

#include <SFML/Audio.hpp>

#include "Resources/Resource.hpp"

namespace PopHead {
namespace Resources {

class SoundBufferResource : public Resource< SoundBufferResource >
{
  private:
    virtual bool load( std::string ) override;

    sf::SoundBuffer mSoundBuffer;
};

}}

#endif // !POPHEAD_RESOURCE_SOUNDBUFFERRESOURCE_H_
