#ifndef POPHEAD_BASE_GAMEDATA_H_
#define POPHEAD_BASE_GAMEDATA_H_

#include <memory>

#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Renderer/renderer.hpp"
#include "States/stateMachine.hpp"
#include "Input/input.hpp"
#include "Resources/resourceHolder.hpp"
#include "Physics/physicsEngine.hpp"
#include "Terminal/terminal.hpp"

namespace ph {

/// GameData is holder for observer pointers to Game Modules.

class GameData
{
public:
	inline GameData();
	inline GameData(
		SoundPlayer* const,
		MusicPlayer* const,
		TextureHolder* const,
		FontHolder* const,
		ShaderHolder* const,
		StateMachine* const,
		Input* const,
		Renderer* const,
		PhysicsEngine* const,
		Terminal* const);

	auto getSoundPlayer()	const -> SoundPlayer & { return *mSoundPlayer; }
	auto getMusicPlayer()	const -> MusicPlayer & { return *mMusicPlayer; }
	auto getTextures()		const -> TextureHolder & { return *mTextures; }
	auto getFonts()			const -> FontHolder & { return *mFonts; }
	auto getShaders()		const -> ShaderHolder & { return *mShaders; }
	auto getStateMachine()	const -> StateMachine & { return *mStateMachine; }
	auto getInput()			const -> Input & { return *mInput; }
	auto getRenderer()		const -> Renderer & { return *mRenderer; }
	auto getPhysicsEngine()	const -> PhysicsEngine & { return *mPhysicsEngine; }
	auto getTerminal()		const -> Terminal & { return *mTerminal; }

private:
	SoundPlayer* const mSoundPlayer;
	MusicPlayer* const mMusicPlayer;
	TextureHolder* const mTextures;
	FontHolder* const mFonts;
	ShaderHolder* const mShaders;
	StateMachine* const mStateMachine;
	Input* const mInput;
	Renderer* const mRenderer;
	PhysicsEngine* const mPhysicsEngine;
	Terminal* const mTerminal;
};

inline GameData::GameData()
	:GameData(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {}

inline GameData::GameData(
	SoundPlayer* const soundPlayer,
	MusicPlayer* const musicPlayer,
	TextureHolder* const textures,
	FontHolder* const fonts,
	ShaderHolder* const shaders,
	StateMachine* const stateMachine,
	Input* const input,
	Renderer* const renderer,
	PhysicsEngine* const physicsEngine,
	Terminal* const Terminal)
	:mSoundPlayer{soundPlayer}
	,mMusicPlayer{musicPlayer}
	,mTextures{textures}
	,mFonts{fonts}
	,mShaders{shaders}
	,mStateMachine{stateMachine}
	,mInput{input}
	,mRenderer{renderer}
	,mPhysicsEngine{physicsEngine}
	,mTerminal{Terminal}
{
}

}

#endif // !POPHEAD_BASE_GAMEDATA_H_
