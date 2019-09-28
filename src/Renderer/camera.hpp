#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class Camera
{
public:
	Camera() = default;

	Camera(sf::Vector2f center, sf::Vector2f size);

	void update(sf::Time delta);

	void setCenter(sf::Vector2f center);
	auto getCenter() const -> sf::Vector2f { return mView.getCenter(); }

	void setSize(sf::Vector2f size) { mView.setSize(size); }
	auto getSize() const -> sf::Vector2f { return mView.getSize(); }

	auto getBounds() const -> sf::FloatRect;

	auto getView() const -> sf::View { return mView; };

	void setViewport(sf::FloatRect viewport) { mView.setViewport(viewport); }

	void setShakeStrength(float shakeStrength) { mShakeStrength = shakeStrength; }
	void setShakeStrengthLoss(float shakeStrengthLoss) { mShakeStrengthLoss = shakeStrengthLoss; }
	void shake(float shakeStrengthLoss);

	void move(sf::Vector2f center, float speed);

	auto getCameraMoveFromLastFrame() -> sf::Vector2f { return mCameraMoveFromLastFrame; }

private:
	void updateCameraMoveFromLastFrame();
	void updateLastCameraPosition();

private:
	sf::View mView;
	sf::Vector2f mCenterWithoutShake;
	float mShakeStrength = 0.f;
	float mShakeStrengthLoss = 10.f;
	sf::Vector2f mLastFrameCameraCenterPosition;
	sf::Vector2f mCameraMoveFromLastFrame;
};

}
