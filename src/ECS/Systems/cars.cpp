#include "cars.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

void Cars::update(float dt)
{
	auto cars = mRegistry.view<component::Car, component::BodyRect>();
	cars.each([dt](component::Car& car, component::BodyRect& body) 
	{
		if(car.shouldSpeedUp)
			car.velocity += car.acceleration * dt;
		if(car.shouldSlowDown)
			car.velocity -= car.acceleration * dt;
		
		constexpr float frictionForce = 0.1f;
		car.velocity -= frictionForce * dt;
		
		if(car.velocity < 1.f)
			car.velocity = 0.f;
		
		body.rect.move(car.direction * car.velocity);
	});
}

}



