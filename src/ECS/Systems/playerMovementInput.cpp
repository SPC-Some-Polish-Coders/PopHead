#include "playerMovementInput.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "AI/aiManager.hpp"
#include "Scenes/scene.hpp"
#include "GUI/gui.hpp"
#include "Utilities/direction.hpp"
#include "Utilities/profiling.hpp"
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
		if(e.type == sf::Event::KeyPressed)
		{
			if(e.key.code == sf::Keyboard::Escape)
			{
				// TODO_states: Pause screen could be handled by states
				auto playerView = mRegistry.view<component::Player, component::Health>();
				playerView.each([this](component::Player, component::Health) {
					sPause ? GUI::hideInterface("pauseScreen") : GUI::showInterface("pauseScreen");
					sPause = !sPause;
				});
			}
			else if(e.key.code == sf::Keyboard::LShift)
			{
				mTimeFromDashPressed = 0.f;
			}
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
		mUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W); 
		mDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S); 
		mLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A); 
		mRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D); 

		// get player direction
		sf::Vector2f playerDirection;
		if(mUp && mLeft)  playerDirection = PH_NORTH_WEST;
		else if(mUp && mRight) playerDirection = PH_NORTH_EAST;
		else if(mDown && mLeft) playerDirection = PH_SOUTH_WEST;
		else if(mDown && mRight) playerDirection = PH_SOUTH_EAST;
		else if(mUp) playerDirection = PH_NORTH;
		else if(mDown) playerDirection = PH_SOUTH;
		else if(mLeft) playerDirection = PH_WEST;
		else if(mRight) playerDirection = PH_EAST;

		for(auto& player : playerView)
		{
			// update animation data
			auto& animationData = playerView.get<component::AnimationData>(player);

			if(mUp && mLeft) {
				animationData.currentStateName = "leftUp";
				animationData.isPlaying = true;
			}
			else if(mUp && mRight) {
				animationData.currentStateName = "rightUp";
				animationData.isPlaying = true;
			}
			else if(mLeft) {
				animationData.currentStateName = "left";
				animationData.isPlaying = true;
			}
			else if(mRight) {
				animationData.currentStateName = "right";
				animationData.isPlaying = true;
			}
			else if(mUp) {
				animationData.currentStateName = "up";
				animationData.isPlaying = true;
			}
			else if(mDown) {
				animationData.currentStateName = "down";
				animationData.isPlaying = true;
			}
			else {
				animationData.isPlaying = false;
			}

			// set face direction
			if (playerDirection != sf::Vector2f(0.f, 0.f))
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
		movementView.each([this, playerDirection]
		(const component::Player, component::Velocity& velocity, const component::CharacterSpeed& speed, const component::BodyRect& body) 
		{
			mAIManager.setPlayerPosition(body.rect.getTopLeft());
			auto vel = playerDirection * speed.speed;
			if(mTimeFromDashPressed < 0.1f)
				vel *= 2.f;
			velocity.dx = vel.x;
			velocity.dy = vel.y;
		});

		mTimeFromDashPressed += dt;
	}
}

