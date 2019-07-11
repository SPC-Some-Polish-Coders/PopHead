#pragma once

#include <memory>

#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "Renderer/renderer.hpp"
#include "Scenes/sceneMachine.hpp"
#include "Map/map.hpp"
#include "Input/input.hpp"
#include "Resources/resourceHolder.hpp"
#include "Physics/physicsEngine.hpp"
#include "Terminal/terminal.hpp"
#include "EfficiencyRegister/efficiencyRegister.hpp"
#include "Gui/GUI.hpp"

namespace ph {

/// GameData is holder for observer pointers to Game Modules.

class GameCloser
{
public:
	void closeTheGame() { mShouldGameBeClosed = true; };
	bool shouldGameBeClosed() { return mShouldGameBeClosed; };

private:
	bool mShouldGameBeClosed = false;
};

class GameData
{
public:
	GameData()
	:GameData(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {}
	
	GameData(
		SoundPlayer* const soundPlayer,
		MusicPlayer* const musicPlayer,
		TextureHolder* const textures,
		FontHolder* const fonts,
		ShaderHolder* const shaders,
		SceneMachine* const sceneMachine,
		Map* const map,
		Input* const input,
		Renderer* const renderer,
		PhysicsEngine* const physicsEngine,
		Terminal* const Terminal,
		EfficiencyRegister* const efficiencyRegister,
		GUI* const Gui
	)
		:mSoundPlayer{soundPlayer}
		,mMusicPlayer{musicPlayer}
		,mTextures{textures}
		,mFonts{fonts}
		,mShaders{shaders}
		,mSceneMachine{sceneMachine}
		,mMap(map)
		,mInput{input}
		,mRenderer{renderer}
		,mPhysicsEngine{physicsEngine}
		,mTerminal{Terminal}
		,mEfficiencyRegister{efficiencyRegister}
		,mGui(Gui)
		,mGameCloser()
	{
	}
	
	auto getSoundPlayer() const -> SoundPlayer& { return *mSoundPlayer; }
	auto getMusicPlayer() const -> MusicPlayer& { return *mMusicPlayer; }
	auto getTextures() const -> TextureHolder& { return *mTextures; }
	auto getFonts()	const -> FontHolder& { return *mFonts; }
	auto getShaders() const -> ShaderHolder& { return *mShaders; }
	auto getSceneMachine() const -> SceneMachine& { return *mSceneMachine; }
	auto getMap() const -> Map& { return *mMap; }
	auto getInput()	const -> Input& { return *mInput; }
	auto getRenderer() const -> Renderer& { return *mRenderer; }
	auto getPhysicsEngine()	const -> PhysicsEngine& { return *mPhysicsEngine; }
	auto getTerminal() const -> Terminal& { return *mTerminal; }
	auto getEfficiencyRegister() const -> EfficiencyRegister& { return *mEfficiencyRegister; }
	auto getGui() const -> GUI& { return *mGui; }
	auto getGameCloser() -> GameCloser& { return mGameCloser; }

private:
	SoundPlayer* const mSoundPlayer;
	MusicPlayer* const mMusicPlayer;
	TextureHolder* const mTextures;
	FontHolder* const mFonts;
	ShaderHolder* const mShaders;
	SceneMachine* const mSceneMachine;
	Map* const mMap;
	Input* const mInput;
	Renderer* const mRenderer;
	PhysicsEngine* const mPhysicsEngine;
	Terminal* const mTerminal;
	EfficiencyRegister* const mEfficiencyRegister;
	GUI* const mGui;
	GameCloser mGameCloser;
};

}
