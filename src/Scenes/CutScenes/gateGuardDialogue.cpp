#include "gateGuardDialogue.hpp"
#include "GUI/gui.hpp"
#include "Events/actionEventManager.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"

namespace ph {

GateGuardDialogue::GateGuardDialogue(entt::registry& gameRegistry, GUI& gui)
	:mRegistry(gameRegistry)
	,mGui(gui)
	,mPlayerOnThePosition(false)
	,mTimesPressedSkip(1)
{
	ActionEventManager::setEnabled(false);
	mGui.hideInterface("gameplayCounters");
	initGui();

	auto cameraView = mRegistry.view<component::Camera>();
	cameraView.each([](component::Camera& cameraDetails) {
			cameraDetails.camera.setSize(sf::Vector2f(426.f, 240.f));
		});
}

void GateGuardDialogue::initGui()
{
	mGui.showInterface("labels");
	auto playerSpeechBubble = mGui.getInterface("labels")->getWidget("canvas")->getWidget("playerSpeechBubble");
	auto guardSpeechBubble = mGui.getInterface("labels")->getWidget("canvas")->getWidget("guardSpeechBubble");
	auto hint = mGui.getInterface("labels")->getWidget("canvas")->getWidget("hint");
	
	playerSpeechBubble->hide();
	guardSpeechBubble->hide();
	hint->hide();

	guardSpeechBubble->getWidget("characterName")->show();
	playerSpeechBubble->getWidget("player1")->hide();
	playerSpeechBubble->getWidget("player2")->hide();

	guardSpeechBubble->getWidget("characterName")->show();
	guardSpeechBubble->getWidget("guard1")->hide();
	guardSpeechBubble->getWidget("guard1b")->hide();
	guardSpeechBubble->getWidget("guard1c")->hide();
	guardSpeechBubble->getWidget("guard2")->hide();
	guardSpeechBubble->getWidget("guard2b")->hide();
	guardSpeechBubble->getWidget("guard2c")->hide();
	guardSpeechBubble->getWidget("guard3")->hide();
	guardSpeechBubble->getWidget("guard3b")->hide();
	guardSpeechBubble->getWidget("guard3c")->hide();
	guardSpeechBubble->getWidget("guard4")->hide();
	guardSpeechBubble->getWidget("guard4b")->hide();
	guardSpeechBubble->getWidget("guard5")->hide();
	guardSpeechBubble->getWidget("guard5b")->hide();
	guardSpeechBubble->getWidget("guard5c")->hide();
	guardSpeechBubble->getWidget("guard5c")->hide();
	guardSpeechBubble->getWidget("guard6")->hide();
	guardSpeechBubble->getWidget("guard6b")->hide();
	guardSpeechBubble->getWidget("guard6c")->hide();
	guardSpeechBubble->getWidget("guard7")->hide();
}
	
void GateGuardDialogue::update(const sf::Time dt)
{
	if (!mPlayerOnThePosition)
		prepareCutScene(dt.asSeconds());

	// TODO: Use events here
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mTimeSinceLastSkipPress.getElapsedTime().asSeconds() > 0.3f)
	{
		++mTimesPressedSkip;
		mTimeSinceLastSkipPress.restart();
	}

	auto guardSpeechBubble = mGui.getInterface("labels")->getWidget("canvas")->getWidget("guardSpeechBubble");
	auto playerSpeechBubble = mGui.getInterface("labels")->getWidget("canvas")->getWidget("playerSpeechBubble");
	auto hint = mGui.getInterface("labels")->getWidget("canvas")->getWidget("hint");

	if(mPlayerOnThePosition) 
	{
		switch(mTimesPressedSkip)
		{
		case 1:
		{
			hint->show();
			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player1")->show();
			playerSpeechBubble->getWidget("player1b")->show();
		} break;

		case 2:
		{
			playerSpeechBubble->hide();
			playerSpeechBubble->getWidget("player1")->hide();
			playerSpeechBubble->getWidget("player1b")->hide();

			guardSpeechBubble->show();
			guardSpeechBubble->getWidget("guard1")->show();
			guardSpeechBubble->getWidget("guard1b")->show();
			guardSpeechBubble->getWidget("guard1c")->show();
		} break;

		case 3:
		{
			guardSpeechBubble->hide();
			guardSpeechBubble->getWidget("guard1")->hide();
			guardSpeechBubble->getWidget("guard1b")->hide();
			guardSpeechBubble->getWidget("guard1c")->hide();

			playerSpeechBubble->show();
			playerSpeechBubble->getWidget("player2")->show();
		} break;

		case 4:
		{
			playerSpeechBubble->hide();
			playerSpeechBubble->getWidget("player2")->hide();

			guardSpeechBubble->show();
			guardSpeechBubble->getWidget("guard2")->show();
			guardSpeechBubble->getWidget("guard2b")->show();
			guardSpeechBubble->getWidget("guard2c")->show();
		} break;

		case 5:
		{
			guardSpeechBubble->getWidget("guard2")->hide();
			guardSpeechBubble->getWidget("guard2b")->hide();
			guardSpeechBubble->getWidget("guard2c")->hide();

			guardSpeechBubble->getWidget("guard3")->show();
			guardSpeechBubble->getWidget("guard3b")->show();
			guardSpeechBubble->getWidget("guard3c")->show();
		} break;

		case 6:
		{
			guardSpeechBubble->getWidget("guard3")->hide();
			guardSpeechBubble->getWidget("guard3b")->hide();
			guardSpeechBubble->getWidget("guard3c")->hide();

			guardSpeechBubble->getWidget("guard4")->show();
			guardSpeechBubble->getWidget("guard4b")->show();
		} break;

		case 7:
		{
			guardSpeechBubble->getWidget("guard4")->hide();
			guardSpeechBubble->getWidget("guard4b")->hide();

			guardSpeechBubble->getWidget("guard5")->show();
			guardSpeechBubble->getWidget("guard5b")->show();
			guardSpeechBubble->getWidget("guard5c")->show();
		} break;

		case 8:
		{
			guardSpeechBubble->getWidget("guard5")->hide();
			guardSpeechBubble->getWidget("guard5b")->hide();
			guardSpeechBubble->getWidget("guard5c")->hide();

			guardSpeechBubble->getWidget("guard6")->show();
			guardSpeechBubble->getWidget("guard6b")->show();
			guardSpeechBubble->getWidget("guard6c")->show();
		} break;

		case 9:
		{
			guardSpeechBubble->getWidget("guard6")->hide();
			guardSpeechBubble->getWidget("guard6b")->hide();
			guardSpeechBubble->getWidget("guard6c")->hide();

			guardSpeechBubble->getWidget("guard7")->show();
		} break;

		default:
			guardSpeechBubble->getWidget("guard7")->hide();
			guardSpeechBubble->hide();
			playerSpeechBubble->hide();
			hint->hide();
			leaveCutScene();
			break;
		}
	}
}

void GateGuardDialogue::prepareCutScene(float dt)
{
	auto playerView = mRegistry.view<component::Player, component::BodyRect, component::AnimationData>();
	for (auto player : playerView)
	{
		auto& playerBody = playerView.get<component::BodyRect>(player);
		if (playerBody.rect.top > 600.f)
			playerBody.rect.top += -20.f * dt;
		else
		{
			auto& animationData = playerView.get<component::AnimationData>(player);
			animationData.currentStateName = "rightUp";
			mPlayerOnThePosition = true;
			auto leverListenerView = mRegistry.view<component::LeverListener>();
			leverListenerView.each([](component::LeverListener& leverListenerDetails) {
				leverListenerDetails.isActivated = true;
				});
		}
	}
}

void GateGuardDialogue::leaveCutScene()
{
	ActionEventManager::setEnabled(true);
	mIsActive = false;
	mGui.showInterface("gameplayCounters");
	auto cameraView = mRegistry.view<component::Camera>();

	cameraView.each([](component::Camera& cameraDetails) {
		cameraDetails.camera.setSize(sf::Vector2f(640.f, 360.f));
		});
}

}
