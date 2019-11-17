#include "efficiencyDisplayer.hpp"
#include "Renderer/renderer.hpp"
#include "gameData.hpp"

namespace ph {

EfficiencyDisplayer::EfficiencyDisplayer()
	:mShouldFPSBeDrawn(false)
	,mShouldRendererDebugBeDrawn(false)
{
}

void EfficiencyDisplayer::init(GameData* const gameData)
{
	mFPSBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mFPSBackground.setPosition(160, -200);
	mFPSBackground.setSize({100, 15});
	
	mRendererDebugBackground.setFillColor(sf::Color(0, 0, 0, 230));
	mRendererDebugBackground.setPosition(0, -185);
	mRendererDebugBackground.setSize({260, 51});

	const sf::Font& font(gameData->getFonts().get("fonts/joystixMonospace.ttf"));

	mFramesPerSecondText.setFont(font);
	mFramesPerSecondText.setPosition(160, -200);
	mFramesPerSecondText.setCharacterSize(10);

	mDrawCallPerFrameText.setFont(font);
	mDrawCallPerFrameText.setPosition(0, -185);
	mDrawCallPerFrameText.setCharacterSize(10);

	mSFMLDrawCalls.setFont(font);
	mSFMLDrawCalls.setPosition(0, -175);
	mSFMLDrawCalls.setCharacterSize(10);

	mInstancedDrawCalls.setFont(font);
	mInstancedDrawCalls.setPosition(0, -165);
	mInstancedDrawCalls.setCharacterSize(10);

	mDrawnInstancedSprites.setFont(font);
	mDrawnInstancedSprites.setPosition(0, -155);
	mDrawnInstancedSprites.setCharacterSize(10);

	mTexturesDrawnByInstancing.setFont(font);
	mTexturesDrawnByInstancing.setPosition(0, -145);
	mTexturesDrawnByInstancing.setCharacterSize(10);
}

void EfficiencyDisplayer::draw() const
{
	if(mShouldFPSBeDrawn) {
		Renderer::submitSFMLObject(mFPSBackground);
		Renderer::submitSFMLObject(mFramesPerSecondText);
	}
		
	if(mShouldRendererDebugBeDrawn) {
		Renderer::submitSFMLObject(mRendererDebugBackground);
		Renderer::submitSFMLObject(mDrawCallPerFrameText);
		Renderer::submitSFMLObject(mSFMLDrawCalls);
		Renderer::submitSFMLObject(mInstancedDrawCalls);
		Renderer::submitSFMLObject(mDrawnInstancedSprites);
		Renderer::submitSFMLObject(mTexturesDrawnByInstancing);
	}
}

void EfficiencyDisplayer::setShouldFPSBeDrawn(const bool shouldBeDrawn)
{
	mShouldFPSBeDrawn = shouldBeDrawn;
}

void EfficiencyDisplayer::setShouldRendererDebugBeDrawn(const bool shouldRendererDebugBeDrawn)
{
	mShouldRendererDebugBeDrawn = shouldRendererDebugBeDrawn;
}

void EfficiencyDisplayer::setFramePerSecondText(const std::string& text)
{
	mFramesPerSecondText.setString(text);
}

void EfficiencyDisplayer::setDrawCallPerFrameText(const std::string& text)
{
	mDrawCallPerFrameText.setString(text);
}

void EfficiencyDisplayer::setSFMLDrawCalls(const std::string& text)
{
	mSFMLDrawCalls.setString(text);
}

void EfficiencyDisplayer::setInstancedDrawCalls(const std::string& text)
{
	mInstancedDrawCalls.setString(text);
}

void EfficiencyDisplayer::setDrawnInstancedSprites(const std::string& text)
{
	mDrawnInstancedSprites.setString(text);
}

void EfficiencyDisplayer::setTexturesDrawnByInstancing(const std::string& text)
{
	mTexturesDrawnByInstancing.setString(text);
}

}
