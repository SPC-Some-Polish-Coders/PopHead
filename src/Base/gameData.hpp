#ifndef POPHEAD_BASE_GAMEDATA_H_
#define POPHEAD_BASE_GAMEDATA_H_

#include <memory>

#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Renderer/renderer.hpp"
#include "States/stateMachine.hpp"
#include "Input/input.hpp"
#include "Resources/resourceHolder.hpp"
#include "Gui/GUI.hpp"

namespace PopHead {
namespace Base {

/// GameData is holder for observer pointers to Game Modules.
/** GameData is recivied by Game itself. */
class GameData
{
  public:
    inline GameData();
    inline GameData(
            Audio::SoundPlayer* const,
            Audio::MusicPlayer* const,
            Resources::TextureHolder* const,
            Resources::FontHolder* const,
            Resources::ShaderHolder* const,
            States::StateMachine* const,
            Input::Input* const,
			GUI::GUI* const,
            Renderer::Renderer* const );

    auto getSoundPlayer()  const -> Audio::SoundPlayer&       { return *mSoundPlayer;  }
    auto getMusicPlayer()  const -> Audio::MusicPlayer&       { return *mMusicPlayer;  }
    auto getTextures()     const -> Resources::TextureHolder& { return *mTextures;     }
    auto getFonts()        const -> Resources::FontHolder&    { return *mFonts;        }
    auto getShaders()      const -> Resources::ShaderHolder&  { return *mShaders;      }
    auto getStateMachine() const -> States::StateMachine&     { return *mStateMachine; }
    auto getInput()        const -> Input::Input&             { return *mInput;        }
    auto getRenderer()     const -> Renderer::Renderer&       { return *mRenderer;     }

  private:
    Audio::SoundPlayer* const mSoundPlayer;
    Audio::MusicPlayer* const mMusicPlayer;
    Resources::TextureHolder* const mTextures;
    Resources::FontHolder* const mFonts;
    Resources::ShaderHolder* const mShaders;
    States::StateMachine* const mStateMachine;
    Input::Input* const mInput;
	GUI::GUI* const mGui;
    Renderer::Renderer* const mRenderer;
};

inline GameData::GameData()
        : GameData( nullptr, nullptr, nullptr, nullptr,
                    nullptr, nullptr, nullptr, nullptr , nullptr) {}

inline GameData::GameData(
        Audio::SoundPlayer* const soundPlayer,
        Audio::MusicPlayer* const musicPlayer,
        Resources::TextureHolder* const textures,
        Resources::FontHolder* const fonts,
        Resources::ShaderHolder* const shaders,
        States::StateMachine* const stateMachine,
        Input::Input* const input,
		GUI::GUI* const gui,
        Renderer::Renderer* const renderer )
        : mSoundPlayer { soundPlayer }
        , mMusicPlayer { musicPlayer }
        , mTextures { textures }
        , mFonts { fonts }
        , mShaders { shaders }
        , mStateMachine { stateMachine }
        , mInput { input }
		, mGui { gui }
        , mRenderer { renderer }
{
}


}}

#endif // !POPHEAD_BASE_GAMEDATA_H_
