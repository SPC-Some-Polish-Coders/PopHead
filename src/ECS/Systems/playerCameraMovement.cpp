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
	auto view = mRegistry.view<component::Player, component::Camera, component::BodyRect>();
		
	view.each([dt](const component::Player, component::Camera& camera, const component::BodyRect& bodyRect) 
	{
		camera.camera.setCenterSmoothly(bodyRect.rect.getCenter(), 4 * dt);
	});
}

}