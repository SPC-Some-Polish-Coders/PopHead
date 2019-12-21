#include "debugCounter.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

DebugCounter::DebugCounter()
	:mFPSCounter()
	,mRendererDebug()
	,mFont()
	,mClock()
	,mFPS(0)
	,mFramesFromLastSecond(0)
	,mIsFPSCounterActive(false)
	,mIsRendererDebugActive(false)
{
}

void DebugCounter::init(FontHolder& fonts)
{
	mFont = &fonts.get("fonts/joystixMonospace.ttf");
}

void DebugCounter::handleEvent(const ph::Event& phEvent)
{
	if(auto* e = std::get_if<sf::Event>(&phEvent))
	{
		if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F2)
		{
			mIsFPSCounterActive = !mIsFPSCounterActive;
			if(mIsFPSCounterActive) 
			{
				mFPSCounter.reset(new FPSCounter);
				initFPSCounter();
			}
			else
				mFPSCounter.reset();
		}
		else if(e->type == sf::Event::KeyPressed && e->key.code == sf::Keyboard::F3)
		{
			mIsRendererDebugActive = !mIsRendererDebugActive;
			if(mIsRendererDebugActive)
			{ 
				mRendererDebug.reset(new RendererDebug);		
				initRendererDebug();
			}
			else
				mRendererDebug.reset();
		}
	}
}

void DebugCounter::initFPSCounter()
{
	mFPSCounter->fpsBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mFPSCounter->fpsBackground.setPosition(160, -200);
	mFPSCounter->fpsBackground.setSize({100, 15});

	mFPSCounter->fpsText.setFont(*mFont);
	mFPSCounter->fpsText.setPosition(160, -200);
	mFPSCounter->fpsText.setCharacterSize(10);
}

void DebugCounter::initRendererDebug()
{
	mRendererDebug->rendererDebugBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mRendererDebug->rendererDebugBackground.setPosition(0, -185);
	mRendererDebug->rendererDebugBackground.setSize({260, 101});

	mRendererDebug->allDrawCallsText.setFont(*mFont);
	mRendererDebug->allDrawCallsText.setPosition(0, -185);
	mRendererDebug->allDrawCallsText.setCharacterSize(10);

	mRendererDebug->sfmlDrawCallsText.setFont(*mFont);
	mRendererDebug->sfmlDrawCallsText.setPosition(0, -175);
	mRendererDebug->sfmlDrawCallsText.setCharacterSize(10);

	mRendererDebug->instancedDrawCallsText.setFont(*mFont);
	mRendererDebug->instancedDrawCallsText.setPosition(0, -165);
	mRendererDebug->instancedDrawCallsText.setCharacterSize(10);

	mRendererDebug->renderGroupsInQuadRendererText.setFont(*mFont);
	mRendererDebug->renderGroupsInQuadRendererText.setPosition(0, -155);
	mRendererDebug->renderGroupsInQuadRendererText.setCharacterSize(10);

	mRendererDebug->drawnInstancedSpritesText.setFont(*mFont);
	mRendererDebug->drawnInstancedSpritesText.setPosition(0, -145);
	mRendererDebug->drawnInstancedSpritesText.setCharacterSize(10);

	mRendererDebug->texturesDrawnByInstancingText.setFont(*mFont);
	mRendererDebug->texturesDrawnByInstancingText.setPosition(0, -135);
	mRendererDebug->texturesDrawnByInstancingText.setCharacterSize(10);

	mRendererDebug->lineDrawCallsText.setFont(*mFont);
	mRendererDebug->lineDrawCallsText.setPosition(0, -125);
	mRendererDebug->lineDrawCallsText.setCharacterSize(10);

	mRendererDebug->drawnLinesText.setFont(*mFont);
	mRendererDebug->drawnLinesText.setPosition(0, -115);
	mRendererDebug->drawnLinesText.setCharacterSize(10);

	mRendererDebug->pointDrawCallsText.setFont(*mFont);
	mRendererDebug->pointDrawCallsText.setPosition(0, -105);
	mRendererDebug->pointDrawCallsText.setCharacterSize(10);

	mRendererDebug->drawnPointsText.setFont(*mFont);
	mRendererDebug->drawnPointsText.setPosition(0, -95);
	mRendererDebug->drawnPointsText.setCharacterSize(10);
}

void DebugCounter::update()
{
	if(mIsFPSCounterActive)
		 mFPSCounter->fpsText.setString("FPS:  " + std::to_string(mFPS));

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
		Renderer::submitSFMLObject(mFPSCounter->fpsBackground);
		Renderer::submitSFMLObject(mFPSCounter->fpsText);
	}

	if(mIsRendererDebugActive) {
		Renderer::submitSFMLObject(mRendererDebug->rendererDebugBackground);
		Renderer::submitSFMLObject(mRendererDebug->allDrawCallsText);
		Renderer::submitSFMLObject(mRendererDebug->sfmlDrawCallsText);
		Renderer::submitSFMLObject(mRendererDebug->instancedDrawCallsText);
		Renderer::submitSFMLObject(mRendererDebug->renderGroupsInQuadRendererText);
		Renderer::submitSFMLObject(mRendererDebug->drawnInstancedSpritesText);
		Renderer::submitSFMLObject(mRendererDebug->texturesDrawnByInstancingText);
		Renderer::submitSFMLObject(mRendererDebug->lineDrawCallsText);
		Renderer::submitSFMLObject(mRendererDebug->drawnLinesText);
		Renderer::submitSFMLObject(mRendererDebug->pointDrawCallsText);
		Renderer::submitSFMLObject(mRendererDebug->drawnPointsText);
	}
}

void DebugCounter::setAllDrawCallsPerFrame(unsigned alldrawCallsPerFrame)
{
	if(mIsRendererDebugActive)
		mRendererDebug->allDrawCallsText.setString("All Draw calls per frame: " + std::to_string(alldrawCallsPerFrame));
}

void DebugCounter::setNumberOfSFMLDrawCalls(unsigned nrOfDrawnSprites)
{
	if(mIsRendererDebugActive)
		mRendererDebug->sfmlDrawCallsText.setString("SFML draw calls: " + std::to_string(nrOfDrawnSprites));
}

void DebugCounter::setNumberOfInstancedDrawCalls(unsigned nrOfInstancedDrawCalls)
{
	if(mIsRendererDebugActive)
		mRendererDebug->instancedDrawCallsText.setString("Instanced draw calls: " + std::to_string(nrOfInstancedDrawCalls));
}

void DebugCounter::setNumberOfRenderGroups(unsigned nrOfRenderGroups)
{
	if(mIsRendererDebugActive)
		mRendererDebug->renderGroupsInQuadRendererText.setString("Render groups in quad renderer: " + std::to_string(nrOfRenderGroups));
}

void DebugCounter::setNumberOfDrawnInstancedSprites(unsigned nrOfDrawnInstancedSprites)
{
	if(mIsRendererDebugActive)
		mRendererDebug->drawnInstancedSpritesText.setString("Drawn instanced sprites: " + std::to_string(nrOfDrawnInstancedSprites));
}

void DebugCounter::setNumberOfTexturesDrawnByInstancedRendering(unsigned nrOfTexturesDrawnByInstancedRendering)
{
	if(mIsRendererDebugActive)
		mRendererDebug->texturesDrawnByInstancingText.setString("Drawn textures by instancing: " + std::to_string(nrOfTexturesDrawnByInstancedRendering));
}

void DebugCounter::setNumberOfLineDrawCalls(unsigned nrOfLineDrawCalls)
{
	if(mIsRendererDebugActive)
		mRendererDebug->lineDrawCallsText.setString("Line draw calls: " + std::to_string(nrOfLineDrawCalls));
}

void DebugCounter::setNumberOfDrawnLines(unsigned nrOfDrawnLines)
{
	if(mIsRendererDebugActive)
		mRendererDebug->drawnLinesText.setString("Drawn lines: " + std::to_string(nrOfDrawnLines));
}

void DebugCounter::setNumberOfDrawnPoints(unsigned nrOfDrawnPoints)
{
	if(mIsRendererDebugActive)
		mRendererDebug->drawnPointsText.setString("Drawn points: " + std::to_string(nrOfDrawnPoints));
}

void DebugCounter::setNumberOfPointDrawCalls(unsigned nrOfDrawCalls)
{
	if(mIsRendererDebugActive)
		mRendererDebug->pointDrawCallsText.setString("Point draw calls: " + std::to_string(nrOfDrawCalls));
}

}
