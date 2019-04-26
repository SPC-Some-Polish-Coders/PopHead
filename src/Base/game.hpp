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
#include "Gui/GUI.hpp"

namespace PopHead {
namespace Base {

/// Main game class.
/** On initialization creates game and run mainloop. */
class Game
{
  public:
    /// Default constructor.
    /** Initializes modules, and runs game. */
    Game();

  private:
    /// Main game method.
    /** mainloop is executed here. */
    void run();

    /// Handle input.
    /** Handling input recivied from SFML. */
    void input();

    /// World update method.
    /** Here whole game logic is resolved.
     *  @param delta - time (in milliseconds) from last frame. */
    void update( sf::Time delta );

    /// Draw method.
    /// Draw current state on screen.
    void draw();

    /// Get GameData.
    /** @return GameData reference. */
    inline auto getGameData() const -> const Base::GameData&;

  ///@{ @name GameData members
    std::unique_ptr< Base::GameData >           mGameData;
    std::unique_ptr< Audio::SoundPlayer >       mSoundPlayer;
    std::unique_ptr< Audio::MusicPlayer >       mMusicPlayer;
    std::unique_ptr< Resources::TextureHolder > mTextures;
    std::unique_ptr< Resources::FontHolder >    mFonts;
    std::unique_ptr< Resources::ShaderHolder >  mShaders;
    std::unique_ptr< States::StateMachine >     mStateMachine;
    std::unique_ptr< Input::Input >             mInput;
    std::unique_ptr< GUI::GUI >                 mGui;
    std::unique_ptr< Renderer::Renderer >       mRenderer; ///@}
};

inline auto Game::getGameData() const -> const Base::GameData&
{ return *(mGameData); }

}}

#endif // !POPHEAD_BASE_GAME_H_
