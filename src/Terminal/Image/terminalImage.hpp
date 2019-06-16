#ifndef POPHEAD_TERMINAL_RENDERER_TERMINALRENDERER_H_
#define POPHEAD_TERMINAL_RENDERER_TERMINALRENDERER_H_

#include <SFML/Graphics.hpp>
#include "Terminal/terminalSharedData.hpp"

namespace ph {

class GameData;

class TerminalImage : public sf::Drawable
{
public:
	TerminalImage(TerminalSharedData);
	void initializeText(GameData*);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void move(sf::Vector2f offset);

private:
	sf::RectangleShape mTerminalBackground;
	TerminalSharedData mTerminalSharedData;
};

}

#endif