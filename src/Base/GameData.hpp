#ifndef POPHEAD_BASE_GAMEDATA_H_
#define POPHEAD_BASE_GAMEDATA_H_

#include <memory>

#include "Audio/Music/MusicPlayer.hpp"
#include "Audio/Sound/SoundPlayer.hpp"
#include "Renderer/Renderer.hpp"
#include "States/StateMachine.hpp"
#include "Input/Input.hpp"
#include "Resources/ResourceHolder.hpp"

namespace PopHead {
namespace Base {

struct GameData
{
    inline GameData(
            Audio::SoundPlayer* const = nullptr,
            Audio::MusicPlayer* const = nullptr,
            Resources::TextureHolder* const = nullptr,
            Resources::FontHolder* const = nullptr,
            Resources::ShaderHolder* const = nullptr,
            States::StateMachie* const = nullptr,
            Input::Input* const = nullptr,
            Renderer::Renderer* const  = nullptr );

    Audio::SoundPlayer* const mSoundPlayer;
    Audio::MusicPlayer* const mMusicPlayer;
    Resources::TextureHolder* const mTextures;
    Resources::FontHolder* const mFonts;
    Resources::ShaderHolder* const mShaders;
    States::StateMachie* const mStateMachine;
    Input::Input* const mInput;
    Renderer::Renderer* const mRenderer;
};

inline GameData::GameData(
        Audio::SoundPlayer* const soundPlayer,
        Audio::MusicPlayer* const musicPlayer,
        Resources::TextureHolder* const textures,
        Resources::FontHolder* const fonts,
        Resources::ShaderHolder* const shaders,
        States::StateMachie* const stateMachine,
        Input::Input* const input,
        Renderer::Renderer* const renderer )
        : mSoundPlayer { soundPlayer }
        , mMusicPlayer { musicPlayer }
        , mTextures { textures }
        , mFonts { fonts }
        , mShaders { shaders }
        , mStateMachine { stateMachine }
        , mInput { input }
        , mRenderer { renderer } {};

}}

#endif // !POPHEAD_BASE_GAMEDATA_H_
