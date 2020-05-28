#include "pch.hpp"
#include "playerMovementInput.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/scene.hpp"
#include "GUI/gui.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/joystickMacros.hpp"

extern bool debugWindowOpen;

namespace ph::system {

	PlayerMovementInput::PlayerMovementInput(entt::registry& registry, AIManager& aiManager, Scene* scene)
		:System(registry)
		,mScene(scene)
		,mAIManager(aiManager)
	{
	}

	void PlayerMovementInput::onEvent(sf::Event e)
	{
		auto doPause = [this]()
		{
			// TODO_states: Pause screen could be handled by states
			mRegistry.view<component::Player, component::Health>().each([this]
			(auto, auto)
			{
				sPause ? GUI::hideInterface("pauseScreen") : GUI::showInterface("pauseScreen");
				sPause = !sPause;
			});
		};

		if(e.type == sf::Event::KeyPressed) 
		{
			if(e.key.code == sf::Keyboard::Escape)
				doPause();
			else if(e.key.code == sf::Keyboard::LShift && !dashInputDisabled)
				mDashJustPressed = true; 
		}
		else if(e.type == sf::Event::JoystickButtonPressed)
		{
			// TODO: Don't forget to update joystick controls
			if(e.joystickButton.button == PH_JOYSTICK_MENU)
				doPause();
			else if(e.joystickButton.button == PH_JOYSTICK_X && !dashInputDisabled)
				mDashJustPressed = true;
		}
	}

	void PlayerMovementInput::update(float dt)
	{
		PH_PROFILE_FUNCTION();

		if(sPause)
			return;

		// return if player is without control
		for(auto player : mRegistry.view<component::Player>())
			if(mRegistry.has<component::DeadCharacter>(player))
				return;

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
		{
			d *= 0.707106781187f;
		}

		mRegistry.view<component::Player, component::AnimationData, component::FaceDirection>().each([=]
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
		mRegistry.view<component::Player, component::FaceDirection, component::LightSource>().each([this]
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

		static float dashLength = 75.f;

		// move player
		mRegistry.view<component::Player, component::Kinematics, component::CharacterSpeed, component::BodyRect>
		(entt::exclude<component::DeadCharacter>).each([&]
		(auto, auto& kinematics, auto speed, auto& body) 
		{
			if(mDashJustPressed) 
			{
				if(mDashes)
				{
					--mDashes;
					body.pos += d * dashLength;
				}
				mDashJustPressed = false;
			}
			else
			{
				kinematics.acceleration = d * speed.speed;
			}

			mAIManager.setPlayerPosition(body.pos);
		});

		static float timeToGetNextDash = 2.f;

		// add dashes
		if(mDashes < 2)
		{
			mTimeToHaveNextDash -= dt;
			if(mTimeToHaveNextDash < 0.f)
			{
				++mDashes;
				mTimeToHaveNextDash = timeToGetNextDash; 
			}
		}

		if(debugWindowOpen && ImGui::BeginTabItem("tunning"))
		{
			ImGui::SliderFloat("dash length", &dashLength, 20.f, 100.f);
			ImGui::SliderFloat("time to get next dash", &timeToGetNextDash, 1.f, 5.f);
			ImGui::EndTabItem();
		}
	}
}

