#pragma once

#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/sceneManager.hpp"
#include "Resources/resourceHolder.hpp"
#include "Physics/physicsEngine.hpp"
#include "Terminal/terminal.hpp"
#include "Gui/GUI.hpp"
#include <SFML/Window/Window.hpp>
#include <memory>

namespace ph {

class GameCloser
{
public:
	void closeGame();
	bool shouldGameBeClosed();

private:
	bool mShouldGameBeClosed = false;
};

/// GameData is holder for observer pointers to Game Modules.

class GameData
{
public:
	GameData()
	:GameData(nullptr, nullptr, nullptr, nullptr, nullptr,
	          nullptr, nullptr, nullptr, nullptr, nullptr) {}
	
	GameData(
		sf::Window* const window,
		SoundPlayer* const soundPlayer,
		MusicPlayer* const musicPlayer,
		TextureHolder* const textures,
		FontHolder* const fonts,
		AIManager* const aiManager,
		SceneManager* const sceneManager,
		PhysicsEngine* const physicsEngine,
		Terminal* const Terminal,
		GUI* const Gui
	)
		:mWindow(window)
		,mSoundPlayer{soundPlayer}
		,mMusicPlayer{musicPlayer}
		,mTextures{textures}
		,mFonts{fonts}
		,mAIMangager(aiManager)
		,mSceneManager{sceneManager}
		,mPhysicsEngine{physicsEngine}
		,mTerminal{Terminal}
		,mGui(Gui)
		,mGameCloser()
	{
	}
	
	auto getWindow() const -> sf::Window& { return *mWindow; }
	auto getSoundPlayer() const -> SoundPlayer& { return *mSoundPlayer; }
	auto getMusicPlayer() const -> MusicPlayer& { return *mMusicPlayer; }
	auto getTextures() const -> TextureHolder& { return *mTextures; }
	auto getFonts()	const -> FontHolder& { return *mFonts; }
	auto getAIManager() const -> AIManager& { return *mAIMangager; }
	auto getSceneManager() const -> SceneManager& { return *mSceneManager; }
	auto getPhysicsEngine()	const -> PhysicsEngine& { return *mPhysicsEngine; }
	auto getTerminal() const -> Terminal& { return *mTerminal; }
	auto getGui() const -> GUI& { return *mGui; }
	auto getGameCloser() -> GameCloser& { return mGameCloser; }

private:
	sf::Window* const mWindow;
	SoundPlayer* const mSoundPlayer;
	MusicPlayer* const mMusicPlayer;
	TextureHolder* const mTextures;
	FontHolder* const mFonts;
	AIManager* const mAIMangager;
	SceneManager* const mSceneManager;
	PhysicsEngine* const mPhysicsEngine;
	Terminal* const mTerminal;
	GUI* const mGui;
	GameCloser mGameCloser;
};

}
