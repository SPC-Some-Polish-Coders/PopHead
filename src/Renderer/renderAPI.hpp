#pragma once

#include <SFML/Graphics.hpp>

namespace ph {

class RenderAPI
{
public:
	enum class API
	{
		LegacyOpenGL, None
	};

	virtual void init() = 0;

	virtual void setClearColor(const sf::Color& color) = 0;
	virtual void clear() = 0;

	virtual void setView(const sf::View&) = 0;

	virtual void draw(const sf::Drawable&, const sf::RenderStates) = 0;

private:
	inline static API mApi = API::LegacyOpenGL;
};

}
