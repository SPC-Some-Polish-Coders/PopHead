#ifndef POPHEAD_BASE_GAME_H_
#define POPHEAD_BASE_GAME_H_

#include <memory>

#include "Audio/Music/MusicPlayer.hpp"
#include "Audio/Sound/SoundPlayer.hpp"
#include "Renderer/Renderer.hpp"
#include "States/StateMachine.hpp"
#include "Input/Input.hpp"
#include "Resources/ResourceHolder.hpp"

namespace PopHead {
namespace Base {

class Game
{
  public:
    Game();

  private:
    void run();
    void input();
    void update( float delta );
    void draw();
    void mainloop();

    std::unique_ptr< Audio::SoundPlayer > mSoundPlayer;
    std::unique_ptr< Audio::MusicPlayer > mMusicPlayer;
    std::unique_ptr< Resources::TextureHolder > mTextures;
    std::unique_ptr< Resources::FontHolder > mFonts;
    std::unique_ptr< Resources::ShaderHolder > mShaders;
    std::unique_ptr< States::StateMachie > mStateMachine;
    std::unique_ptr< Input::Input > mInput;
    std::unique_ptr< Renderer::Renderer > mRenderer;
};
}}

#endif // !POPHEAD_BASE_GAME_H_
