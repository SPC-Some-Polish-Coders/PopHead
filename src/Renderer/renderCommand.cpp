#include "renderCommand.hpp"
#include "LegacyOpenGl/legacyOpenGlRenderAPI.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

RenderCommand::RenderCommand()
	:mRenderAPI(std::make_unique<LegacyOpenGLRenderAPI>())
{
}

void RenderCommand::init()
{
	mRenderAPI->init();
}

void RenderCommand::setClearColor(const sf::Color& color)
{
	mRenderAPI->setClearColor(color);
}

void RenderCommand::clear()
{
	mRenderAPI->clear();
}

void RenderCommand::setView(const sf::View& view)
{
	mRenderAPI->setView(view);
}

void RenderCommand::draw(const sf::Drawable& drawable, const sf::RenderStates states)
{
	mRenderAPI->draw(drawable, states);
}

}
