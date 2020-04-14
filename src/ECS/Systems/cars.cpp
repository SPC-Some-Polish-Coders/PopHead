#include "cars.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "Utilities/profiling.hpp"

namespace ph::system {

void Cars::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause)
		return;

	auto cars = mRegistry.view<component::Car, component::BodyRect>();
	cars.each([dt](component::Car& car, component::BodyRect& body) 
	{
		if(car.shouldSpeedUp)
			car.velocity += car.acceleration * dt;
		if(car.shouldSlowDown)
			car.velocity -= car.slowingDown * dt;
		
		constexpr float frictionForce = 0.1f;
		car.velocity -= frictionForce * dt;
		
		if(car.velocity < 1.f)
			car.velocity = 0.f;
		
		body.rect.move(car.direction * car.velocity * dt);
	});
}

}



