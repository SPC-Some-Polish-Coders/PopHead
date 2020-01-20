#pragma once

#include "Resources/resourceHolder.hpp"
#include <string>

namespace ph {

class GUI;
class SceneManager;
class GameCloser;
class MusicPlayer;
class SoundPlayer;

void parseGuiXml(const std::string& fileName, GUI& gui, TextureHolder& textureHolder, SceneManager& sceneManager,
                 GameCloser& gameCloser, MusicPlayer& musicPlayer, SoundPlayer& soundPlayer);

}
