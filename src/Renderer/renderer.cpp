#include "Renderer/renderer.hpp"
#include "GameObjects/gameObject.hpp"
#include "Logs/logs.hpp"
#include "Utilities/math.hpp"
#include "gameData.hpp"

namespace ph {

Renderer::Renderer(sf::RenderTarget& renderTarget)
	:mRenderTarget(renderTarget)
	,mCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mStaticObjectsCamera{ sf::Vector2f{0,0}, sf::Vector2f{16*40, 16*30} }
	,mViewports{ { FullScreenViewport, { 0.f, 0.f, 1.f, 1.f } } }
	,mGameData(nullptr)
	,mDebugRenderingMode(false)
	,mSceneTreeRoot(nullptr)
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

void Renderer::draw() const
{
	mCamera.applyTo(mRenderTarget);
	mRenderTarget.clear();

	sf::FloatRect properCameraBounds = getProperCameraBounds();
	mGameData->getMap().draw(mRenderTarget, sf::RenderStates::Default, properCameraBounds);
	mSceneTreeRoot->draw(mRenderTarget, sf::RenderStates::Default);
	//drawSceneLayers(properCameraBounds);
	mRenderTarget.draw(mGameData->getPhysicsEngine().getCollisionDebugManager());
	drawStaticObjectsToCamera();
}

//void Renderer::drawSceneLayers(sf::FloatRect properCameraBounds) const
//{
//	for(const auto& layer : mLayers)
//		for(const auto& drawableGameObject : layer.second) {
//			auto objectPosition = drawableGameObject->getPosition();
//			sf::FloatRect objectBounds(objectPosition.x - 100, objectPosition.y - 100, 200, 200);
//			if(Math::areTheyOverlapping(properCameraBounds, objectBounds)) {
//				mRenderTarget.draw(*drawableGameObject);
//				mGameData->getEfficiencyRegister().registerDrawCall();
//			}
//		}
//}

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

void Renderer::drawStaticObjectsToCamera() const
{
	mStaticObjectsCamera.applyTo(mRenderTarget);
	mRenderTarget.draw(mGameData->getGui().getGuiDrawer());
	mRenderTarget.draw(mGameData->getEfficiencyRegister().getDisplayer());
	mRenderTarget.draw(mGameData->getTerminal().getImage());
}

}