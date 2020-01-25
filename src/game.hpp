#pragma once

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
	Terminal* getTerminal() { return mTerminal.get(); }

	static void close() { sIsRunning = false; }
	static void setNoFocusUpdate(bool flag) { sNoFocusUpdate = flag; }

private:
	void handleEvents();
	void update(float dt);

private:
	sf::Window                     mWindow;
	FPSCounter                     mFPSCounter;
	std::unique_ptr<TextureHolder> mTextures;
	std::unique_ptr<AIManager>     mAIManager;
	std::unique_ptr<SceneManager>  mSceneManager;
	std::unique_ptr<Terminal>      mTerminal;

	inline static bool sIsRunning = true;
	inline static bool sNoFocusUpdate = false;
};

}
