#include "debugCounter.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

DebugCounter::DebugCounter()
	:mClock()
	,mFPS(0)
	,mFramesFromLastSecond(0)
	,mIsFPSCounterActive(false)
	,mIsRendererDebugActive(false)
{
}

void DebugCounter::init(FontHolder& fonts)
{
	mFPSBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mFPSBackground.setPosition(160, -200);
	mFPSBackground.setSize({100, 15});

	mRendererDebugBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mRendererDebugBackground.setPosition(0, -185);
	mRendererDebugBackground.setSize({260, 101});

	const sf::Font& font(fonts.get("fonts/joystixMonospace.ttf"));

	mFPSText.setFont(font);
	mFPSText.setPosition(160, -200);
	mFPSText.setCharacterSize(10);

	mAllDrawCallsText.setFont(font);
	mAllDrawCallsText.setPosition(0, -185);
	mAllDrawCallsText.setCharacterSize(10);

	mSFMLDrawCallsText.setFont(font);
	mSFMLDrawCallsText.setPosition(0, -175);
	mSFMLDrawCallsText.setCharacterSize(10);

	mInstancedDrawCallsText.setFont(font);
	mInstancedDrawCallsText.setPosition(0, -165);
	mInstancedDrawCallsText.setCharacterSize(10);

	mRenderGroupsInQuadRendererText.setFont(font);
	mRenderGroupsInQuadRendererText.setPosition(0, -155);
	mRenderGroupsInQuadRendererText.setCharacterSize(10);

	mDrawnInstancedSpritesText.setFont(font);
	mDrawnInstancedSpritesText.setPosition(0, -145);
	mDrawnInstancedSpritesText.setCharacterSize(10);

	mTexturesDrawnByInstancingText.setFont(font);
	mTexturesDrawnByInstancingText.setPosition(0, -135);
	mTexturesDrawnByInstancingText.setCharacterSize(10);

	mLineDrawCallsText.setFont(font);
	mLineDrawCallsText.setPosition(0, -125);
	mLineDrawCallsText.setCharacterSize(10);

	mDrawnLinesText.setFont(font);
	mDrawnLinesText.setPosition(0, -115);
	mDrawnLinesText.setCharacterSize(10);

	mPointDrawCallsText.setFont(font);
	mPointDrawCallsText.setPosition(0, -105);
	mPointDrawCallsText.setCharacterSize(10);

	mDrawnPointsText.setFont(font);
	mDrawnPointsText.setPosition(0, -95);
	mDrawnPointsText.setCharacterSize(10);
}

void DebugCounter::handleEvent(const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F2)
			mIsFPSCounterActive = !mIsFPSCounterActive;
		else if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F3)
			mIsRendererDebugActive = !mIsRendererDebugActive;
	}
}

void DebugCounter::update()
{
	if(mIsFPSCounterActive)
		mFPSText.setString("FPS:  " + std::to_string(mFPS));

	if(mClock.getElapsedTime().asSeconds() >= 1) {
		mFPS = mFramesFromLastSecond;
		mClock.restart();
		mFramesFromLastSecond = 0;
	}
	else
		++mFramesFromLastSecond;
}

void DebugCounter::draw() const
{
	if(mIsFPSCounterActive) {
		Renderer::submitSFMLObject(mFPSBackground);
		Renderer::submitSFMLObject(mFPSText);
	}

	if(mIsRendererDebugActive) {
		Renderer::submitSFMLObject(mRendererDebugBackground);
		Renderer::submitSFMLObject(mAllDrawCallsText);
		Renderer::submitSFMLObject(mSFMLDrawCallsText);
		Renderer::submitSFMLObject(mInstancedDrawCallsText);
		Renderer::submitSFMLObject(mRenderGroupsInQuadRendererText);
		Renderer::submitSFMLObject(mDrawnInstancedSpritesText);
		Renderer::submitSFMLObject(mTexturesDrawnByInstancingText);
		Renderer::submitSFMLObject(mLineDrawCallsText);
		Renderer::submitSFMLObject(mDrawnLinesText);
		Renderer::submitSFMLObject(mPointDrawCallsText);
		Renderer::submitSFMLObject(mDrawnPointsText);
	}
}

void DebugCounter::setAllDrawCallsPerFrame(unsigned alldrawCallsPerFrame)
{
	if(mIsRendererDebugActive)
		mAllDrawCallsText.setString("All Draw calls per frame: " + std::to_string(alldrawCallsPerFrame));
}

void DebugCounter::setNumberOfSFMLDrawCalls(unsigned nrOfDrawnSprites)
{
	if(mIsRendererDebugActive)
		mSFMLDrawCallsText.setString("SFML draw calls: " + std::to_string(nrOfDrawnSprites));
}

void DebugCounter::setNumberOfInstancedDrawCalls(unsigned nrOfInstancedDrawCalls)
{
	if(mIsRendererDebugActive)
		mInstancedDrawCallsText.setString("Instanced draw calls: " + std::to_string(nrOfInstancedDrawCalls));
}

void DebugCounter::setNumberOfRenderGroups(unsigned nrOfRenderGroups)
{
	if(mIsRendererDebugActive)
		mRenderGroupsInQuadRendererText.setString("Render groups in quad renderer: " + std::to_string(nrOfRenderGroups));
}

void DebugCounter::setNumberOfDrawnInstancedSprites(unsigned nrOfDrawnInstancedSprites)
{
	if(mIsRendererDebugActive)
		mDrawnInstancedSpritesText.setString("Drawn instanced sprites: " + std::to_string(nrOfDrawnInstancedSprites));
}

void DebugCounter::setNumberOfTexturesDrawnByInstancedRendering(unsigned nrOfTexturesDrawnByInstancedRendering)
{
	if(mIsRendererDebugActive)
		mTexturesDrawnByInstancingText.setString("Drawn textures by instancing: " + std::to_string(nrOfTexturesDrawnByInstancedRendering));
}

void DebugCounter::setNumberOfLineDrawCalls(unsigned nrOfLineDrawCalls)
{
	if(mIsRendererDebugActive)
		mLineDrawCallsText.setString("Line draw calls: " + std::to_string(nrOfLineDrawCalls));
}

void DebugCounter::setNumberOfDrawnLines(unsigned nrOfDrawnLines)
{
	if(mIsRendererDebugActive)
		mDrawnLinesText.setString("Drawn lines: " + std::to_string(nrOfDrawnLines));
}

void DebugCounter::setNumberOfDrawnPoints(unsigned nrOfDrawnPoints)
{
	if(mIsRendererDebugActive)
		mDrawnPointsText.setString("Drawn points: " + std::to_string(nrOfDrawnPoints));
}

void DebugCounter::setNumberOfPointDrawCalls(unsigned nrOfDrawCalls)
{
	if(mIsRendererDebugActive)
		mPointDrawCallsText.setString("Point draw calls: " + std::to_string(nrOfDrawCalls));
}

}
