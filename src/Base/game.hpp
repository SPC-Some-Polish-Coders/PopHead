#ifndef POPHEAD_BASE_GAME_H_
#define POPHEAD_BASE_GAME_H_

#include <memory>

#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Base/gameData.hpp"
#include "Renderer/renderer.hpp"
#include "States/stateMachine.hpp"
#include "Input/input.hpp"
#include "Resources/resourceHolder.hpp"
#include "Physics/physicsEngine.hpp"
#include "Logs/logger.hpp"

namespace PopHead {
namespace Base {


class Game
{
  public:
    Game();

    void run();
  private:
    void input();
    void update( sf::Time delta );
    void draw();

	inline auto getGameData() const -> const Base::GameData& { return *(mGameData); };

    std::unique_ptr< Base::GameData >           mGameData;
    std::unique_ptr< Audio::SoundPlayer >       mSoundPlayer;
    std::unique_ptr< Audio::MusicPlayer >       mMusicPlayer;
    std::unique_ptr< Resources::TextureHolder > mTextures;
    std::unique_ptr< Resources::FontHolder >    mFonts;
    std::unique_ptr< Resources::ShaderHolder >  mShaders;
    std::unique_ptr< States::StateMachine >     mStateMachine;
    std::unique_ptr< Input::Input >             mInput;
    std::unique_ptr< Renderer::Renderer >       mRenderer; 
	std::unique_ptr< Physics::PhysicsEngine >	mPhysicsEngine;
};


}}

#endif // !POPHEAD_BASE_GAME_H_
