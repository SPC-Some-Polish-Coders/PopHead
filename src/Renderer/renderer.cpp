#include "Renderer/renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "Logs/logs.hpp"
#include "Utilities/math.hpp"
#include "Map/map.hpp"

namespace ph {

Renderer::Renderer(sf::RenderTarget& renderTarget)
	:mRenderTarget(renderTarget)
	,mCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mStaticObjectsCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mViewports{ { FullScreenViewport, { 0.f, 0.f, 1.f, 1.f } } }
	,mDebugRenderingMode(false)
{
	mCamera.setViewport(mViewports.at(FullScreenViewport));
}

Renderer::~Renderer()
{
}

void Renderer::update(sf::Time delta)
{
	mCamera.update(delta);
}

void Renderer::startSceneRendering()
{
	mProperCameraBounds = getProperCameraBounds();
	mRenderTarget.clear();
	mCamera.applyTo(mRenderTarget);
}

void Renderer::startUIRendering()
{
	mStaticObjectsCamera.applyTo(mRenderTarget);
}

void Renderer::draw(const sf::Drawable& drawableObject) const
{
	mRenderTarget.draw(drawableObject);
}

void Renderer::draw(const Map& map) const
{
	map.draw(mRenderTarget, sf::RenderStates::Default, mProperCameraBounds);
}

sf::FloatRect Renderer::getProperCameraBounds() const
{
	if(mDebugRenderingMode) {
		sf::Vector2f cameraCommonSize(640, 480);
		const sf::Vector2f cameraCommonLeftCornerPosition(
			mCamera.getCenter().x - cameraCommonSize.x / 2,
			mCamera.getCenter().y - cameraCommonSize.x / 2
		);
		return sf::FloatRect(cameraCommonLeftCornerPosition.x, cameraCommonLeftCornerPosition.y,
			cameraCommonSize.x, cameraCommonSize.y);
	}
	else
		return mCamera.getBounds();
}

}
