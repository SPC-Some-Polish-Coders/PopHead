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
	mRendererDebugBackground.setPosition(60, -185);
	mRendererDebugBackground.setSize({200, 35});

	const sf::Font& font(gameData->getFonts().get("fonts/joystixMonospace.ttf"));

	mFramesPerSecondText.setFont(font);
	mFramesPerSecondText.setPosition(160, -200);
	mFramesPerSecondText.setCharacterSize(10);

	mDrawCallPerFrameText.setFont(font);
	mDrawCallPerFrameText.setPosition(60, -185);
	mDrawCallPerFrameText.setCharacterSize(10);

	mNumberOfDrawnSprites.setFont(font);
	mNumberOfDrawnSprites.setPosition(60, -165);
	mNumberOfDrawnSprites.setCharacterSize(10);
}

void EfficiencyDisplayer::draw() const
{
	if(mShouldFPSBeDrawn) {
		Renderer::submit(mFPSBackground);
		Renderer::submit(mFramesPerSecondText);
	}
		
	if(mShouldRendererDebugBeDrawn) {
		Renderer::submit(mRendererDebugBackground);
		Renderer::submit(mDrawCallPerFrameText);
		Renderer::submit(mNumberOfDrawnSprites);
	}
}

void EfficiencyDisplayer::move(sf::Vector2f offset)
{
	mFPSBackground.move(offset);
	mFramesPerSecondText.move(offset);
	mRendererDebugBackground.move(offset);
	mDrawCallPerFrameText.move(offset);
	mNumberOfDrawnSprites.move(offset);
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

void EfficiencyDisplayer::setNumberOfDrawnSpritesText(const std::string& text)
{
	mNumberOfDrawnSprites.setString(text);
}

}
