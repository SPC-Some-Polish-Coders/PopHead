#include <SFML/Audio/SoundBuffer.hpp>
#include <string>

namespace ph {

bool loadSoundBuffer(const std::string& filepath);
sf::SoundBuffer& getSoundBuffer(const std::string& filepath);
void clearSoundBufferHolder();

}
