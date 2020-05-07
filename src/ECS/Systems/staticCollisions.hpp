#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

namespace ph::component {
	struct KinematicCollisionBody;
}

namespace ph::system {

	class StaticCollisions : public System
	{
	public:
		using System::System;

		void update(float dt) override;

	private:
		void calculateStaticCollisions();
		void handleStaticCollision(const ph::FloatRect& staticBody, ph::FloatRect& kinematicBody, component::KinematicCollisionBody& collision);

		void calculateKinematicCollisions();
	};
}
