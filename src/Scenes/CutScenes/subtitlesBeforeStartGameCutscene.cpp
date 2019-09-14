#include "subtitlesBeforeStartGameCutscene.hpp"
#include "Scenes/sceneManager.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

SubtitlesBeforeStartGameCutscene::SubtitlesBeforeStartGameCutscene(GameObject& root, SceneManager& sceneManager)
	:CutScene(root)
	,mSceneManager(sceneManager)
{
}

void SubtitlesBeforeStartGameCutscene::update(sf::Time delta)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		mIsActive = false;
		mSceneManager.replaceScene("scenes/startGameCutscene.xml");
	}
}

}
