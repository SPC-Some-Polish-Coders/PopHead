#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"
#include "ECS/Components/physicsComponents.hpp"

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
		Vec2 handleCollision(const FloatRect& staticRect, const component::BodyCircle* staticCircle, FloatRect& kinematicRect,
		                     const component::BodyCircle* kinematicCircle, entt::entity kinematicEntity, component::KinematicCollisionBody& kinematicBody);

		void calculateKinematicCollisions();

		Vec2 getCollisionVector(const FloatRect& staticRect, const component::BodyCircle* staticCircle, 
		                        const FloatRect& kinematicRect, const component::BodyCircle* kinematicCircle) const;

		void resetKinematicBody(component::KinematicCollisionBody& kinematicBody);

	private:
		struct CollisionInfo
		{
			entt::entity entity;
			bool isCircle;

			CollisionInfo(entt::entity entity, bool isCircle)
				: entity(entity), isCircle(isCircle) {}
		};
		std::vector<CollisionInfo> pushedLeft;
		std::vector<CollisionInfo> pushedRight;
		std::vector<CollisionInfo> pushedUp;
		std::vector<CollisionInfo> pushedDown;
	};
}
