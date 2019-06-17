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
#include "Terminal/terminal.hpp"

namespace ph {

class Game
{
public:
	Game();

	void run();
private:
	void input();
	void update(sf::Time delta);
	void draw();

	inline auto getGameData() const -> const GameData& { return *(mGameData); };

	std::unique_ptr< GameData >			mGameData;
	std::unique_ptr< SoundPlayer >		mSoundPlayer;
	std::unique_ptr< MusicPlayer >		mMusicPlayer;
	std::unique_ptr< TextureHolder >	mTextures;
	std::unique_ptr< FontHolder >		mFonts;
	std::unique_ptr< ShaderHolder >		mShaders;
	std::unique_ptr< StateMachine >		mStateMachine;
	std::unique_ptr< Input >			mInput;
	std::unique_ptr< Renderer >			mRenderer;
	std::unique_ptr< PhysicsEngine >	mPhysicsEngine;
	std::unique_ptr< Terminal >			mTerminal;
	std::unique_ptr< GUI >				mGui;
};

}

#endif // !POPHEAD_BASE_GAME_H_
