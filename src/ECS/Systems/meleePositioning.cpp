#include "meleePositioning.hpp"
#include "meleePositioning.hpp"

#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"


namespace ph::system {

void MeleePositioning::update(float seconds)
{

	auto playerView = mRegistry.view<component::MeleeAttacker, component::Player, component::BodyRect, component::FaceDirection>();
	for (auto player : playerView)
	{
		const auto& [playerFaceDirection, meleeAttacker, playerBody] = playerView.get<component::FaceDirection, component::MeleeAttacker, component::BodyRect>(player);

		auto meleeView = mRegistry.view<component::CurrentMeleeWeapon, component::BodyRect, component::RenderQuad>();
		for (auto melee : meleeView)
		{
			auto& currentMelee = meleeView.get<component::CurrentMeleeWeapon>(melee);
			auto& meleeBody = meleeView.get<component::BodyRect>(melee);
			auto& renderQuadMelee = meleeView.get<component::RenderQuad>(melee);

			updateMeleeSpritePosition(playerFaceDirection.direction, playerBody.rect.getTopLeft(), meleeBody.rect);
			updateTexture(meleeAttacker.isAttacking);
		}
	}
}

void MeleePositioning::updateMeleeSpritePosition(const sf::Vector2f& playerFaceDirection, const sf::Vector2f& playerPosition, FloatRect& gunBody)
{
	const sf::Vector2f rightHandPosition = getRightHandPosition(playerFaceDirection) + playerPosition;
	const sf::Vector2f meleeBodySize = gunBody.getSize();
	if (playerFaceDirection == sf::Vector2f(1.f, 0.f))
		gunBody = FloatRect(rightHandPosition + sf::Vector2f(0.f, -3.f), meleeBodySize);
	else if (playerFaceDirection == sf::Vector2f(-1.f, 0.f))
		gunBody = FloatRect(rightHandPosition + sf::Vector2f(3.f, -3.f), meleeBodySize);
	else if (playerFaceDirection == sf::Vector2f(0.f, -1.f))
		gunBody = FloatRect(rightHandPosition + sf::Vector2f(-3.f, 1.f), meleeBodySize);
	else if (playerFaceDirection == sf::Vector2f(0.f, 1.f))
		gunBody = FloatRect(rightHandPosition + sf::Vector2f(-3.f, 9.f), meleeBodySize);
	else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f))
		gunBody = FloatRect(rightHandPosition + sf::Vector2f(0.f, -8.f), meleeBodySize);
	else if (playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
		gunBody = FloatRect(rightHandPosition + sf::Vector2f(0.f, -8.f), meleeBodySize);
	else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f))
		gunBody = FloatRect(rightHandPosition + sf::Vector2f(3.f, -3.f), meleeBodySize);
	else if (playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
		gunBody = FloatRect(rightHandPosition + sf::Vector2f(-3.f, -1.f), meleeBodySize);
	else
		gunBody = FloatRect(rightHandPosition, meleeBodySize);
}

void MeleePositioning::updateTexture(bool isAttacking)
{
	auto meleeView = mRegistry.view<component::CurrentMeleeWeapon>();
	for (auto melee : meleeView)
	{
		auto& currentMelee = meleeView.get<component::CurrentMeleeWeapon>(melee);

		if (!isAttacking)
			mRegistry.assign_or_replace<component::HiddenForRenderer>(melee);
		else
			if (mRegistry.has<component::HiddenForRenderer>(melee))
				mRegistry.remove<component::HiddenForRenderer>(melee);
	}
}

sf::Vector2f MeleePositioning::getRightHandPosition(const sf::Vector2f& playerFaceDirection)
{
	if (playerFaceDirection == sf::Vector2f(1, 0))
		return { 16, 7 };
	else if (playerFaceDirection == sf::Vector2f(-1, 0))
		return { -2, 7 };
	else if (playerFaceDirection == sf::Vector2f(0, 1))
		return { 10, 18 };
	else if (playerFaceDirection == sf::Vector2f(0, -1))
		return { 9, -12 };
	else if (playerFaceDirection == sf::Vector2f(0.7f, -0.7f))
		return { 15, -5 };
	else if (playerFaceDirection == sf::Vector2f(-0.7f, -0.7f))
		return { -1, -1 };
	else if (playerFaceDirection == sf::Vector2f(0.7f, 0.7f))
		return { 17, 17 };
	else if (playerFaceDirection == sf::Vector2f(-0.7f, 0.7f))
		return { -3, 17 };
	else
		return { 16, 7 };
}



}