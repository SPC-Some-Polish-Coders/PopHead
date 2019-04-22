#ifndef POPHEAD_RENDERER_CAMERA_H_
#define POPHEAD_RENDERER_CAMERA_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Renderer {

class Camera {
public:
	Camera() = default;

	Camera(sf::Vector2f center, sf::Vector2f size);

	void setCenter(sf::Vector2f center);
	auto getCenter() const -> sf::Vector2f { return mView.getCenter(); }

	void setSize(sf::Vector2f size) { mView.setSize(size); }
	auto getSize() const -> sf::Vector2f { mView.getSize(); }

	void setViewport(sf::FloatRect viewport) { mView.setViewport(viewport); }

	void setShakeStrength(float shakeStrength) { mShakeStrength = shakeStrength; }
	void shake(float shakeStrengthLoss);

	void move(sf::Vector2f center, float speed);

	void applyTo(sf::RenderTarget& renderTarget) const;

private:
	sf::View mView;
	sf::Vector2f mCenterWithoutShake;
	float mShakeStrength = 0.f;
};

}
}
#endif
