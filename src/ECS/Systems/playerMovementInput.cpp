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
			auto playerView = mRegistry.view<component::Player, component::Health>();
			playerView.each([this](component::Player, component::Health) {
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

		auto playerView = mRegistry.view<component::Player, component::AnimationData, component::FaceDirection>();

		// return if player is without control
		for(auto player : playerView)
			if(mRegistry.has<component::DeadCharacter>(player))
				return;

		// set input variables
		sf::Vector2f d;
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
		sf::Vector2f playerDirection;
		if(d.x < 0.f && d.y < 0.f) { 
			playerDirection = PH_NORTH_WEST;
			d.x = d.y = (d.x + d.y) / 2.f;			
		}
		else if(d.x > 0.f && d.y < 0.f) {
			playerDirection = PH_NORTH_EAST;
			float offset = (d.x + (-d.y)) / 2.f;
			d.x = offset;
			d.y = -offset;
		}
		else if(d.x < 0.f && d.y > 0.f) {
			playerDirection = PH_SOUTH_WEST;
			float offset = ((-d.x) + d.y) / 2.f;
			d.x = -offset;
			d.y = offset;
		}
		else if(d.x > 0.f && d.y > 0.f) {
			playerDirection = PH_SOUTH_EAST;
			d.x = d.y = (d.x + d.y) / 2.f;			
		}
		else if(d.y < 0.f) {
			playerDirection = PH_NORTH;
		}
		else if(d.y > 0.f) {
			playerDirection = PH_SOUTH;
		}
		else if(d.x < 0.f) {
			playerDirection = PH_WEST;
		}
		else if(d.x > 0.f) {
			playerDirection = PH_EAST;
		}

		if((d.x != 0.f) && (d.y != 0.f))
		{
			d *= 0.707106781187f;
		}

		for(auto& player : playerView)
		{
			// update animation data
			auto& animationData = playerView.get<component::AnimationData>(player);

			animationData.isPlaying = true;
			if(d.x < 0.f && d.y < 0.f) {
				animationData.currentStateName = "leftUp";
			}
			else if(d.x > 0.f && d.y < 0.f) {
				animationData.currentStateName = "rightUp";
			}
			else if(d.x < 0.f) {
				animationData.currentStateName = "left";
			}
			else if(d.x > 0.f) {
				animationData.currentStateName = "right";
			}
			else if(d.y < 0.f) {
				animationData.currentStateName = "up";
			}
			else if(d.y > 0.f) {
				animationData.currentStateName = "down";
			}
			else {
				animationData.isPlaying = false;
			}

			// set face direction
			if(playerDirection != sf::Vector2f(0.f, 0.f))
			{
				auto& faceDirection = playerView.get<component::FaceDirection>(player);
				faceDirection.direction = playerDirection;
			}
		}

		// set flash light direction
		auto view = mRegistry.view<component::Player, component::FaceDirection, component::LightSource>();
		view.each([this](const component::Player, const component::FaceDirection face, component::LightSource& lightSource) 
		{
			float middleAngle;
			if(face.direction == PH_EAST)            middleAngle = 0.f;
			else if(face.direction == PH_WEST)       middleAngle = 180.f;
			else if(face.direction == PH_SOUTH)      middleAngle = 90.f;
			else if(face.direction == PH_NORTH)      middleAngle = -90.f;
			else if(face.direction == PH_SOUTH_EAST) middleAngle = 45.f;
			else if(face.direction == PH_NORTH_EAST) middleAngle = -45.f;
			else if(face.direction == PH_SOUTH_WEST) middleAngle = 135.f;
			else if(face.direction == PH_NORTH_WEST) middleAngle = -135.f;
			else middleAngle = 0.f;

			lightSource.startAngle = middleAngle - 35.f;
			lightSource.endAngle = middleAngle + 35.f;
		});

		static float maxDash = 85.f;

		// move player
		auto movementView = mRegistry.view<component::Player, component::Kinematics, component::CharacterSpeed, component::BodyRect>(entt::exclude<component::DeadCharacter>);
		movementView.each([this, d, playerDirection]
		(const component::Player, component::Kinematics& kinematics, component::CharacterSpeed& speed, component::BodyRect& body) 
		{
			if(mDashJustPressed) 
			{
				body.pos += d * mDashMomentum; 
				mDashMomentum = 0.f;
				mDashJustPressed = false;
			}
			else
			{
				kinematics.acceleration = d * speed.speed;
			}

			mAIManager.setPlayerPosition(body.pos);

			if(debugWindowOpen && ImGui::BeginTabItem("tunning"))
			{
				ImGui::Text("dash momentum: %f", mDashMomentum);
				ImGui::SliderFloat("max dash", &maxDash, 50.f, 350.f);
				ImGui::SliderFloat("player movement speed", &speed.speed, 500.f, 3000.f);
				ImGui::SliderFloat("player default friction", &kinematics.defaultFriction, 1.f, 30.f);
				ImGui::Text("Player velocity: %f %f", kinematics.vel.x, kinematics.vel.y);
				ImGui::Text("Player friction: %f", kinematics.friction);
				ImGui::EndTabItem();
			}
		});

		if(mDashMomentum < maxDash)
		{
			float toCenterDash = abs(maxDash / 2.f - mDashMomentum); 
			if(toCenterDash < maxDash / 10.f)
				mDashMomentum += maxDash / 20.f * dt;
			mDashMomentum += dt * toCenterDash;
		}
	}
}

