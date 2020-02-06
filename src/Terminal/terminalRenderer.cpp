#include "terminalRenderer.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

TerminalRenderer::TerminalRenderer(TerminalSharedData terminalSharedData)
	:mTerminalSharedData(terminalSharedData)
{
}

void TerminalRenderer::pushOutputLine(const OutputLine& line)
{
	if(mOutputLines.size() >= 14)
		mOutputLines.pop_back();
	mOutputLines.emplace_front(line);
}

void TerminalRenderer::update() const
{
	if(mTerminalSharedData->isVisible) 
	{
		Renderer::submitQuad(nullptr, nullptr, &sf::Color(0, 0, 0, 230), nullptr, {0.f, 660.f}, {1920.f, 420.f}, 5, 0.f, {},
			ProjectionType::gui, false);

		Renderer::submitQuad(nullptr, nullptr, &sf::Color::Black, nullptr, {0.f, 720.f}, {1920.f, 5.f},
			4, 0.f, {}, ProjectionType::gui, false);

		Renderer::submitText(mTerminalSharedData->content.c_str(), "LiberationMono-Bold.ttf", {5.f, 660.f},
			50.f, sf::Color::White, 0, ProjectionType::gui, false);

		float posY = 723.f;
		for(size_t i = 0; i < mOutputLines.size(); ++i, posY += 25.f)
			Renderer::submitText(mOutputLines[i].mText.c_str(), "LiberationMono.ttf", {5.f, posY}, 25.f, mOutputLines[i].mColor, 0, ProjectionType::gui, false);
	}
}

}

