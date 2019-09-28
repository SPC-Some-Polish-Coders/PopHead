#include "legacyOpenGlRenderAPI.hpp"

namespace ph {

void LegacyOpenGLRenderAPI::init()
{
}

void LegacyOpenGLRenderAPI::setClearColor(const sf::Color& color)
{
	mClearColor = color;
}

void LegacyOpenGLRenderAPI::clear()
{
	mRenderTarget->clear(mClearColor);
}

void LegacyOpenGLRenderAPI::setView(const sf::View& view)
{
	mRenderTarget->setView(view);
}

void LegacyOpenGLRenderAPI::draw(const sf::Drawable& drawable, const sf::RenderStates states)
{
	mRenderTarget->draw(drawable, states);
}

}
