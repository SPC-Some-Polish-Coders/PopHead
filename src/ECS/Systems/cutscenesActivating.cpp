#include "cutscenesActivating.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "Scenes/cutSceneManager.hpp"
#include "Scenes/CutScenes/endingCutscene.hpp"
#include "Scenes/CutScenes/gateGuardDialogue.hpp"
#include "Scenes/CutScenes/startGameCutscene.hpp"
#include "Scenes/CutScenes/subtitlesBeforeStartGameCutscene.hpp"

namespace ph::system {

CutScenesActivating::CutScenesActivating(entt::registry& registry, CutSceneManager& csm, GUI& gui, MusicPlayer& mp, SoundPlayer& sp,
                                         AIManager& ai, SceneManager& sm)
	:System(registry)
	,mCutSceneManager(csm)
	,mGui(gui)
	,mMusicPlayer(mp)
	,mSoundPlayer(sp)
	,mAIManager(ai)
	,mSceneManager(sm)
{
}

void CutScenesActivating::update(float dt)
{
	// get player position
	FloatRect playerBodyRect;
	auto players = mRegistry.view<component::Player, component::BodyRect>();
	players.each([&playerBodyRect](const component::Player, const component::BodyRect& body) {
		playerBodyRect = body.rect;
	});

	// activate cutscenes
	auto cutscenes = mRegistry.view<component::CutScene, component::BodyRect>();
	cutscenes.each([this, playerBodyRect](component::CutScene& cutscene, const component::BodyRect& body) {
		if(cutscene.wasActivated)
			return;
		if(cutscene.isStartingCutSceneOnThisMap || body.rect.contains(playerBodyRect.getCenter())) {
			cutscene.wasActivated = true;
			activateCutscene(cutscene.name);
		}
	});
}

void CutScenesActivating::activateCutscene(const std::string& name) const
{
	if(name == "subtitlesBeforeStartGameCutScene") {
		mCutSceneManager.activateCutscene(std::make_unique<SubtitlesBeforeStartGameCutscene>(mSceneManager, mGui));
	}
	else if(name == "startGameCutScene") {
		mCutSceneManager.activateCutscene(std::make_unique<StartGameCutScene>(mRegistry, mSoundPlayer, mMusicPlayer, mGui, mSceneManager));
	}
	else if(name == "gateGuardDialogue") {
		mCutSceneManager.activateCutscene(std::make_unique<GateGuardDialogue>(mRegistry, mGui));
	}
	else if(name == "endingDialogue") {
		/*auto cameras = mRegistry.view<component::Camera, component::BodyRect>();
		mCutSceneManager.activateCutscene(std::make_unique<EndingCutScene>(mGui, mMusicPlayer, ));*/
	}
}

}