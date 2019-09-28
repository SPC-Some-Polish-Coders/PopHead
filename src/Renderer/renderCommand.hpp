#pragma once

#include "renderAPI.hpp"
#include <SFML/Graphics/Color.hpp>
#include <memory>

namespace ph {

class RenderCommand
{
public:
	RenderCommand();

	void init();

	void setClearColor(const sf::Color&);
	void clear();

	void setView(const sf::View&);

	void draw(const sf::Drawable&, const sf::RenderStates);
	
private:
	std::unique_ptr<RenderAPI> mRenderAPI;
};

}
