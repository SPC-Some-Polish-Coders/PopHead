#include "subtitlesBeforeStartGameCutscene.hpp"
#include "Scenes/sceneManager.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

SubtitlesBeforeStartGameCutscene::SubtitlesBeforeStartGameCutscene(GameObject& root, SceneManager& sceneManager)
	:CutScene(root)
	,mSceneManager(sceneManager)
	,mTimeFromStartOfCutscene(sf::seconds(0.f))
{
}

void SubtitlesBeforeStartGameCutscene::update(sf::Time delta)
{
	mTimeFromStartOfCutscene += delta;
	if(mTimeFromStartOfCutscene > sf::seconds(3.f) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		mIsActive = false;
		mSceneManager.replaceScene("scenes/startGameCutscene.xml");
	}
}

}
