#pragma once

#include "Renderer/renderAPI.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

// Legacy OpenGL RenderAPI works on top of SFML graphics module which is written on top of OpenGl 2.0

class LegacyOpenGLRenderAPI : public RenderAPI
{
public:
	void init() override;

	void setClearColor(const sf::Color& color) override;
	void clear() override;

	void setView(const sf::View&) override;

	void draw(const sf::Drawable&, const sf::RenderStates) override;

	static void setRenderTarget(sf::RenderTarget* target) { mRenderTarget = target; }

private:
	inline static sf::RenderTarget* mRenderTarget;
	sf::Color mClearColor;
};

}
