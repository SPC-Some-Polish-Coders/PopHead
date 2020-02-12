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
#include "Utilities/profiling.hpp"
#include <SFML/Window/Joystick.hpp>
#include <cmath>

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

		if(e.type == sf::Event::KeyPressed) {
			if(e.key.code == sf::Keyboard::Escape)
				doPause();
			else if(e.key.code == sf::Keyboard::LShift)
				mTimeFromDashPressed = 0.f;
		}
		else if(e.type == sf::Event::JoystickButtonPressed){
			if(e.joystickButton.button == PH_JOYSTICK_MENU)
				doPause();
			else if(e.joystickButton.button == PH_JOYSTICK_X)
				mTimeFromDashPressed = 0.f;
		}
	}

	void PlayerMovementInput::update(float dt)
	{
		PH_PROFILE_FUNCTION(0);

		if(sPause)
			return;

		auto playerView = mRegistry.view<component::Player, component::AnimationData, component::FaceDirection>();

		// return if player is without control
		for(auto player : playerView)
			if(mRegistry.has<component::DeadCharacter>(player))
				return;

		// set input variables
		float x = 0.f;
		float y = 0.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			x -= 1.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			x += 1.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			y -= 1.f;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			y += 1.f;
		if(sf::Joystick::isConnected(0) && x == 0.f && y == 0.f)
		{
			auto processThumbInput = [&x, &y](float inX, float inY)
			{
				constexpr float deadZoneThreshold = 65.f;
				x = (inX > deadZoneThreshold || inX < -deadZoneThreshold) ? inX / 100.f : 0.f;
				y = (inY > deadZoneThreshold || inY < -deadZoneThreshold) ? inY / 100.f : 0.f;
			};
			float leftThumbX = sf::Joystick::getAxisPosition(0, PH_JOYSTICK_LEFT_THUMB_X);
			float leftThumbY = sf::Joystick::getAxisPosition(0, PH_JOYSTICK_LEFT_THUMB_Y);
			processThumbInput(leftThumbX, leftThumbY);

			if(x == 0.f && y == 0.f)
			{
				float dPadX = sf::Joystick::getAxisPosition(0, PH_JOYSTICK_DPAD_X);
				float dPadY = -sf::Joystick::getAxisPosition(0, PH_JOYSTICK_DPAD_Y);
				processThumbInput(dPadX, dPadY);
			}
		}

		// get player direction and correct diagonal input
		sf::Vector2f playerDirection;
		if(x < 0.f && y < 0.f) { 
			playerDirection = PH_NORTH_WEST;
			x = y = (x + y) / 2.f;			
		}
		else if(x > 0.f && y < 0.f) {
			playerDirection = PH_NORTH_EAST;
			float offset = (x + (-y)) / 2.f;
			x = offset;
			y = -offset;
		}
		else if(x < 0.f && y > 0.f) {
			playerDirection = PH_SOUTH_WEST;
			float offset = ((-x) + y) / 2.f;
			x = -offset;
			y = offset;
		}
		else if(x > 0.f && y > 0.f) {
			playerDirection = PH_SOUTH_EAST;
			x = y = (x + y) / 2.f;			
		}
		else if(y < 0.f) {
			playerDirection = PH_NORTH;
		}
		else if(y > 0.f) {
			playerDirection = PH_SOUTH;
		}
		else if(x < 0.f) {
			playerDirection = PH_WEST;
		}
		else if(x > 0.f) {
			playerDirection = PH_EAST;
		}

		for(auto& player : playerView)
		{
			// update animation data
			auto& animationData = playerView.get<component::AnimationData>(player);

			// TODO:
			if(x < 0.f && y < 0.f) {
				animationData.currentStateName = "leftUp";
				animationData.isPlaying = true;
			}
			else if(x > 0.f && y < 0.f) {
				animationData.currentStateName = "rightUp";
				animationData.isPlaying = true;
			}
			else if(x < 0.f) {
				animationData.currentStateName = "left";
				animationData.isPlaying = true;
			}
			else if(x > 0.f) {
				animationData.currentStateName = "right";
				animationData.isPlaying = true;
			}
			else if(y < 0.f) {
				animationData.currentStateName = "up";
				animationData.isPlaying = true;
			}
			else if(y > 0.f) {
				animationData.currentStateName = "down";
				animationData.isPlaying = true;
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
			// TODO: Try to do that with std::atan2f() function instead of if statements

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

		// move player
		auto movementView = mRegistry.view<component::Player, component::Velocity, component::CharacterSpeed, component::BodyRect>();
		movementView.each([this, x, y, playerDirection]
		(const component::Player, component::Velocity& velocity, const component::CharacterSpeed& speed, const component::BodyRect& body) 
		{
			mAIManager.setPlayerPosition(body.rect.getTopLeft());
			float dashFactor = mTimeFromDashPressed < 0.1f ? 2.f : 1.f;
			velocity.dx = x * dashFactor * speed.speed;
			velocity.dy = y * dashFactor * speed.speed;
		});

		mTimeFromDashPressed += dt;
	}
}

