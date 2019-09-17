#include "endingCutscene.hpp"
#include "Gui/gui.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Renderer/camera.hpp"

namespace ph {

EndingCutScene::EndingCutScene(GameObject& root, GUI& gui, MusicPlayer& musicPlayer, Camera& camera)
	:CutScene(root)
	,mGui(gui)
	,mMusicPlayer(musicPlayer)
	,mCamera(camera)
{
}

void EndingCutScene::update(const sf::Time delta)
{

}

}