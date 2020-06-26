#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"
#include <SFML/System/Vector2.hpp>

namespace ph::system {

class GunAttacks : public System
{
public:
	using System::System;

	void update(float dt) override;
	void onEvent(sf::Event) override;

private:
	void handlePendingGunAttacks() const;
	Vec2 getBulletStartingPosition(Vec2 playerFaceDirection) const;
	void tagEnemiesInGunAttackArea(Vec2 playerFaceDirection, FloatRect playerBody, Vec2 gunSize,
								   float range, float deflectionAngle, bool* wasOpponentHitOut) const;
	std::vector<Vec2> performShoot(Vec2 playerFaceDirection, Vec2 startingBulletPos, float range,
								   float deflectionAngle, i16 damage, i16 numberOfBullets) const;
	Vec2 getBulletDirection(Vec2 playerFaceDirection, float deflection) const;
	Vec2 getCurrentPosition(Vec2 bulletDirection, Vec2 startingPos, i32 bulletDistance) const;
	void clearInGunAttackAreaTags() const;

	void createShotImage(Vec2 shotsStartingPosition, const std::vector<Vec2>& shots, const std::string& soundFilename) const;
	void handleLastingBullets() const;
	void shakeCamera(float magnitude, bool smooth) const; 

public:
	inline static bool shootInputDisabled = false;
	inline static bool changeWeaponInputDisabled = false;
};

}
