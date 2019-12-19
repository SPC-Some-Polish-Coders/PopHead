#include "isInPlayerAttackArea.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

#include "Utilities/math.hpp"

namespace ph::system {

void IsInPlayerAttackArea::update(float dt)
{
	handleGun();
	handleMelee();
}

void IsInPlayerAttackArea::handleGun() const
{
	auto playerView = mRegistry.view<component::Player, component::BodyRect, component::FaceDirection>();
	auto enemiesView = mRegistry.view<component::Killable, component::BodyRect>();
	auto currentGunView = mRegistry.view<component::CurrentGun, component::GunProperties>();
	for (auto player : playerView)
	{
		const auto& [playerBody, faceDirection] = playerView.get<component::BodyRect, component::FaceDirection>(player);
		for (auto currentGun : currentGunView)
		{
			const auto& gunProperties = currentGunView.get<component::GunProperties>(currentGun);
			FloatRect attackArea;
			attackArea.left = playerBody.rect.getCenter().x * faceDirection.direction.x;

			if(faceDirection.direction == sf::Vector2f(-1.f, 0.f))
				attackArea.left += gunProperties.range;

			attackArea.top = playerBody.rect.getCenter().y + gunProperties.range* faceDirection.direction.y;
			attackArea.height = gunProperties.range * std::tan(Math::degreesToRadians(gunProperties.deflectionAngle))*2.f;
			attackArea.width = gunProperties.range;
			attackArea.top -= attackArea.height / 2;
			
			//auto e = mRegistry.create();
			//mRegistry.assign<component::BodyRect>(e, attackArea);
			//mRegistry.assign<component::RenderQuad>(e, nullptr, nullptr, sf::Color::Red, 0.f, unsigned char('95'), false);
			//mRegistry.assign<component::Lifetime>(e, 0.05f);
		}
	}
}

void IsInPlayerAttackArea::handleMelee() const
{

}

}