#ifndef POPHEAD_RESOURCE_SOUNDBUFFERRESOURCE_H_
#define POPHEAD_RESOURCE_SOUNDBUFFERRESOURCE_H_

#include <SFML/Audio.hpp>

#include "Resources/Resource.hpp"

namespace PopHead {
namespace Resources {

class SoundBufferResource : public Resource< SoundBufferResource >
{
  private:
    virtual bool load() override;

    sf::SoundBuffer mSoundBuffer;
};

}}

#endif // !POPHEAD_RESOURCE_SOUNDBUFFERRESOURCE_H_
