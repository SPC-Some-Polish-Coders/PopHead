#include "Base/Game.hpp"

using PopHead::Base::Game;

Game::Game()
        : mSoundPlayer { new Audio::SoundPlayer() }
        , mMusicPlayer { new Audio::MusicPlayer() }
        , mTextures { new Resources::TextureHolder() }
        , mFonts { new Resources::FontHolder() }
        , mShaders { new Resources::ShaderHolder() }
        , mStateMachine { new States::StateMachie() }
        , mInput { new Input::Input() }
        , mRenderer { new Renderer::Renderer() }
{
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
