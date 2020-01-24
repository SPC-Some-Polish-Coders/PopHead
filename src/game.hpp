#pragma once

#include "gameData.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/sceneManager.hpp"
#include "Resources/resourceHolder.hpp"
#include "Terminal/terminal.hpp"
#include "FPSCounter/fpsCounter.hpp"
#include <SFML/Window/Window.hpp>
#include <memory>

namespace ph {

class Game
{
public:
	Game();

	void run();
	inline auto getGameData() const -> const GameData & { return *(mGameData); };
	Terminal* getTerminal() { return mTerminal.get(); }

	static void setNoFocusUpdate(bool flag) { sNoFocusUpdate = flag; }

private:
	void handleEvents();
	void update(float dt);

private:
	sf::Window                     mWindow;
	FPSCounter                     mFPSCounter;
	std::unique_ptr<GameData>      mGameData;
	std::unique_ptr<TextureHolder> mTextures;
	std::unique_ptr<AIManager>     mAIManager;
	std::unique_ptr<SceneManager>  mSceneManager;
	std::unique_ptr<Terminal>      mTerminal;
	std::unique_ptr<GUI>           mGui;

	inline static bool sNoFocusUpdate = false;
};

}
