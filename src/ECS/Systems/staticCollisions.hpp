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
		sf::Vector2f handleCollision(const ph::FloatRect& staticRect, const component::BodyCircle* staticCircle, ph::FloatRect& kinematicRect,
							 const component::BodyCircle* kinematicCircle, entt::entity kinematicEntity, component::KinematicCollisionBody& kinematicBody);

		void calculateKinematicCollisions();

		sf::Vector2f getCollisionVector(const ph::FloatRect& staticRect, const component::BodyCircle* staticCircle, 
										const ph::FloatRect& kinematicRect, const component::BodyCircle* kinematicCircle) const;

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
