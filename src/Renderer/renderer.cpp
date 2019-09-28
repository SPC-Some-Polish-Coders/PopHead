#include "renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "Logs/logs.hpp"
#include "Utilities/math.hpp"
#include "Map/map.hpp"

namespace ph {

Renderer::Renderer(sf::RenderTarget& renderTarget)
	:mRenderTarget(renderTarget)
	,mCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mStaticObjectsCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mDebugRenderingMode(false)
{
	mCamera.setViewport({0.f, 0.f, 1.f, 1.f});
}

void Renderer::startSceneRendering()
{
	mRenderCommand.clear();
	mRenderCommand.setView(mCamera.getView());
}

void Renderer::startUIRendering()
{
	mRenderCommand.setView(mStaticObjectsCamera.getView());
}

void Renderer::draw(const sf::Drawable& drawableObject)
{
	mRenderCommand.draw(drawableObject, sf::RenderStates::Default);
}

void Renderer::draw(const Map& map)
{
	map.draw(mRenderTarget, sf::RenderStates::Default, getProperCameraBounds());
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
