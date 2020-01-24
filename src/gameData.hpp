#pragma once

#include "Audio/Music/musicPlayer.hpp"
#include "Audio/Sound/soundPlayer.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/sceneManager.hpp"
#include "Resources/resourceHolder.hpp"
#include "Terminal/terminal.hpp"
#include "GUI/gui.hpp"
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
	:GameData(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr) {}
	
	GameData(
		sf::Window* const window,
		MusicPlayer* const musicPlayer,
		TextureHolder* const textures,
		AIManager* const aiManager,
		SceneManager* const sceneManager,
		Terminal* const Terminal,
		GUI* const Gui
	)
		:mWindow(window)
		,mMusicPlayer{musicPlayer}
		,mTextures{textures}
		,mAIMangager(aiManager)
		,mSceneManager{sceneManager}
		,mTerminal{Terminal}
		,mGui(Gui)
		,mGameCloser()
	{
	}
	
	auto getWindow() const -> sf::Window& { return *mWindow; }
	auto getMusicPlayer() const -> MusicPlayer& { return *mMusicPlayer; }
	auto getTextures() const -> TextureHolder& { return *mTextures; }
	auto getAIManager() const -> AIManager& { return *mAIMangager; }
	auto getSceneManager() const -> SceneManager& { return *mSceneManager; }
	auto getTerminal() const -> Terminal& { return *mTerminal; }
	auto getGui() const -> GUI& { return *mGui; }
	auto getGameCloser() -> GameCloser& { return mGameCloser; }

private:
	sf::Window* const mWindow;
	MusicPlayer* const mMusicPlayer;
	TextureHolder* const mTextures;
	AIManager* const mAIMangager;
	SceneManager* const mSceneManager;
	Terminal* const mTerminal;
	GUI* const mGui;
	GameCloser mGameCloser;
};

}

