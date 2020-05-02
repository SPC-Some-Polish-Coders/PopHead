#include "pch.hpp"
#include "playerCameraMovement.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

PlayerCameraMovement::PlayerCameraMovement(entt::registry& registry)
	:System(registry)
{
}

void PlayerCameraMovement::update(float dt)
{
	PH_PROFILE_FUNCTION();

	auto view = mRegistry.view<component::Player, component::Camera, component::BodyRect, component::FaceDirection>();
	view.each([dt](const component::Player, component::Camera& camera, const component::BodyRect& body, const component::FaceDirection face) {
		sf::Vector2f currentCameraCenter = camera.camera.getCenter(); 
		camera.camera.setCenter(Math::lerp(currentCameraCenter, body.rect.getCenter() + face.direction * 20.f, 6.f * dt));
	});
}

}

