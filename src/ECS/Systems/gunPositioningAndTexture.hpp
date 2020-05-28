#pragma once

#include "ECS/system.hpp"
#include "Utilities/rect.hpp"

#include <SFML/System/Vector2.hpp>

namespace ph::system {

class GunPositioningAndTexture : public System
{
public:
	using System::System;

	void update(float dt) override;

private:
	void updateTexture(float dt, Vec2 playerFaceDirection, bool wantToAttack) const;

	void updateGunPosition(Vec2 playerFaceDirection, const FloatRect& playerBody) const;
	Vec2 getGunSpriteFlipping(Vec2 playerFaceDirection, Vec2 gunSize) const;
	Vec2 getGunNewSpritePosition(Vec2 playerFaceDirection, const FloatRect& playerBody, const FloatRect& gunBody) const;
	Vec2 getGunOffset(Vec2 playerFaceDirection, Vec2 playerBodySize) const;


};

}
