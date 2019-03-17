#include "Base/Game.hpp"

using PopHead::Base::Game;

Game::Game()
        : mGameData {}
        , mSoundPlayer { new Audio::SoundPlayer() }
        , mMusicPlayer { new Audio::MusicPlayer() }
        , mTextures { new Resources::TextureHolder() }
        , mFonts { new Resources::FontHolder() }
        , mShaders { new Resources::ShaderHolder() }
        , mStateMachine { new States::StateMachie() }
        , mInput { new Input::Input() }
        , mRenderer { new Renderer::Renderer() }
{
    mGameData.reset( new GameData(
            mSoundPlayer.get(),
            mMusicPlayer.get(),
            mTextures.get(),
            mFonts.get(),
            mShaders.get(),
            mStateMachine.get(),
            mInput.get(),
            mRenderer.get() ) );
    run();
}

void Game::run()
{
    mainloop();
}

void Game::input()
{

};

void Game::update( float delta )
{

};

void Game::draw()
{

};

void Game::mainloop()
{
    while (true)
    {
        input();
        update( 0.f );
        draw();
    };
}
