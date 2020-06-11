#include "pch.hpp"
#include "playerMovementInput.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/entityUtil.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/scene.hpp"
#include "GUI/gui.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/joystickMacros.hpp"

extern bool debugWindowOpen;

static float dashTime = 0.2f; 
static float minTimeBetweenDashes = 0.1f; 
static float dashVel = 220.f;

namespace ph::system {

PlayerMovementInput::PlayerMovementInput(entt::registry& registry, AIManager& aiManager, Scene* scene)
	:System(registry)
	,mScene(scene)
	,mAIManager(aiManager)
{
}

using namespace component;

void PlayerMovementInput::onEvent(sf::Event e)
{
	auto doPause = [this]()
	{
		// TODO_states: Pause screen could be handled by states
		mRegistry.view<Player, Health>().each([this]
		(auto, auto)
		{
			sPause ? GUI::hideInterface("pauseScreen") : GUI::showInterface("pauseScreen");
			sPause = !sPause;
		});
	};

	bool dashInputIsViable = !dashInputDisabled && mTimeToHaveNextDash == 0.f;

	if(e.type == sf::Event::KeyPressed) 
	{
		if(e.key.code == sf::Keyboard::Escape)
			doPause();
		else if(e.key.code == sf::Keyboard::Space && dashInputIsViable)
			mDashButtonJustPressed = true;
	}
	else if(e.type == sf::Event::JoystickButtonPressed)
	{
		// TODO: Don't forget to update joystick controls
		if(e.joystickButton.button == PH_JOYSTICK_MENU)
			doPause();
		else if(e.joystickButton.button == PH_JOYSTICK_X && dashInputIsViable)
			mDashButtonJustPressed = true;
	}
}

void PlayerMovementInput::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause || !isPlayerAlive() || isPlayerFallingIntoPit()) return;

	// set input variables
	Vec2 d;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		d.x -= 1.f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		d.x += 1.f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		d.y -= 1.f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		d.y += 1.f;
	if(sf::Joystick::isConnected(0) && d.x == 0.f && d.y == 0.f)
	{
		auto processThumbInput = [&d](float inX, float inY)
		{
			constexpr float deadZoneThreshold = 65.f;
			d.x = (inX > deadZoneThreshold || inX < -deadZoneThreshold) ? inX / 100.f : 0.f;
			d.y = (inY > deadZoneThreshold || inY < -deadZoneThreshold) ? inY / 100.f : 0.f;
		};
		float leftThumbX = sf::Joystick::getAxisPosition(0, PH_JOYSTICK_LEFT_THUMB_X);
		float leftThumbY = sf::Joystick::getAxisPosition(0, PH_JOYSTICK_LEFT_THUMB_Y);
		processThumbInput(leftThumbX, leftThumbY);

		if(d.x == 0.f && d.y == 0.f)
		{
			float dPadX = sf::Joystick::getAxisPosition(0, PH_JOYSTICK_DPAD_X);
			float dPadY = -sf::Joystick::getAxisPosition(0, PH_JOYSTICK_DPAD_Y);
			processThumbInput(dPadX, dPadY);
		}
	}

	// get player direction and correct diagonal input
	Vec2 playerDir;
	if(d.x < 0.f && d.y < 0.f) 
	{ 
		playerDir = PH_NORTH_WEST;
		d.x = d.y = (d.x + d.y) / 2.f;			
	}
	else if(d.x > 0.f && d.y < 0.f) 
	{
		playerDir = PH_NORTH_EAST;
		float offset = (d.x + (-d.y)) / 2.f;
		d.x = offset;
		d.y = -offset;
	}
	else if(d.x < 0.f && d.y > 0.f) 
	{
		playerDir = PH_SOUTH_WEST;
		float offset = ((-d.x) + d.y) / 2.f;
		d.x = -offset;
		d.y = offset;
	}
	else if(d.x > 0.f && d.y > 0.f) 
	{
		playerDir = PH_SOUTH_EAST;
		d.x = d.y = (d.x + d.y) / 2.f;			
	}
	else if(d.y < 0.f) 
	{
		playerDir = PH_NORTH;
	}
	else if(d.y > 0.f) 
	{
		playerDir = PH_SOUTH;
	}
	else if(d.x < 0.f) 
	{
		playerDir = PH_WEST;
	}
	else if(d.x > 0.f) 
	{
		playerDir = PH_EAST;
	}

	if((d.x != 0.f) && (d.y != 0.f))
		d *= 0.707106781187f;

	mRegistry.view<Player, AnimationData, FaceDirection>().each([=]
	(auto, auto& animationData, auto& faceDir)
	{
		// update animation data
		animationData.isPlaying = true;
		if(d.x < 0.f && d.y < 0.f)
			animationData.currentStateName = "leftUp";
		else if(d.x > 0.f && d.y < 0.f)
			animationData.currentStateName = "rightUp";
		else if(d.x < 0.f)
			animationData.currentStateName = "left";
		else if(d.x > 0.f)
			animationData.currentStateName = "right";
		else if(d.y < 0.f)
			animationData.currentStateName = "up";
		else if(d.y > 0.f)
			animationData.currentStateName = "down";
		else 
			animationData.isPlaying = false;

		// set faceDir
		if(playerDir != Vec2(0.f, 0.f))
			faceDir = playerDir;
	});

	// set flash light direction
	mRegistry.view<Player, FaceDirection, LightSource>().each([this]
	(auto, auto faceDir, auto& lightSource) 
	{
		float middleAngle;
		if(faceDir == PH_EAST)            middleAngle = 0.f;
		else if(faceDir == PH_WEST)       middleAngle = 180.f;
		else if(faceDir == PH_SOUTH)      middleAngle = 90.f;
		else if(faceDir == PH_NORTH)      middleAngle = -90.f;
		else if(faceDir == PH_SOUTH_EAST) middleAngle = 45.f;
		else if(faceDir == PH_NORTH_EAST) middleAngle = -45.f;
		else if(faceDir == PH_SOUTH_WEST) middleAngle = 135.f;
		else if(faceDir == PH_NORTH_WEST) middleAngle = -135.f;
		else middleAngle = 0.f;

		lightSource.startAngle = middleAngle - 35.f;
		lightSource.endAngle = middleAngle + 35.f;
	});

	if(mTimeToHaveNextDash > 0.f)
		mTimeToHaveNextDash -= dt;
	else
		mTimeToHaveNextDash = 0.f;

	// move player
	mRegistry.view<Player, Kinematics, CharacterSpeed, BodyRect, BodyCircle>
	(entt::exclude<DeadCharacter>).each([&]
	(auto entity, auto, auto& kinematics, auto speed, auto& body, auto circle) 
	{
		if(mTimeFromStartingDash > dashTime)
		{
			mTimeFromStartingDash = 0.f;
			mRegistry.remove<CurrentlyDashing>(entity);
		}

		if(mDashButtonJustPressed)
		{
			mDashButtonJustPressed = false;
			mTimeFromStartingDash = 0.0001f;
			mTimeToHaveNextDash = minTimeBetweenDashes + dashTime;
			mDashDirection = playerDir;
			mRegistry.assign<CurrentlyDashing>(entity);
		}

		if(mTimeFromStartingDash > 0.f)
		{
			body.pos += mDashDirection * dashVel * dt;
			mTimeFromStartingDash += dt;
		}
		else
		{
			kinematics.acceleration = d * speed.speed;
		}

		mAIManager.setPlayerPosition(getCirclePos(body, circle));
	});

	if(debugWindowOpen && ImGui::BeginTabItem("tunning"))
	{
		ImGui::SliderFloat("dash vel", &dashVel, 20.f, 400.f);
		ImGui::SliderFloat("min time between dashes", &minTimeBetweenDashes, 0.3f, 5.f);
		ImGui::SliderFloat("dash time", &dashTime, 0.1f, 3.f);
		ImGui::Text("time to have next dash: %f", mTimeToHaveNextDash);
		ImGui::EndTabItem();
	}
}

}

