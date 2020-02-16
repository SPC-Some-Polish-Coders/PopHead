#pragma once

#include "AI/aiManager.hpp"
#include "Scenes/sceneManager.hpp"
#include "Resources/resourceHolder.hpp"
#include "FPSCounter/fpsCounter.hpp"
#include <SFML/Window/Window.hpp>
#include <memory>

namespace ph {

class Game
{
public:
	Game();

	void run();

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

	inline static bool sIsRunning = true;
	inline static bool sNoFocusUpdate = false;
};

}
