#ifndef POPHEAD_TERMINAL_RENDERER_TERMINALRENDERER_H_
#define POPHEAD_TERMINAL_RENDERER_TERMINALRENDERER_H_

#include "Terminal/terminalSharedData.hpp"
#include "outputArea.hpp"

#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class TerminalImage : public sf::Drawable
{
public:
	TerminalImage(TerminalSharedData);
	void init(GameData*);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void move(sf::Vector2f offset);

private:
	void initializeText(GameData*);

private:
	OutputArea mOutputArea;
	sf::RectangleShape mBackground;
	sf::RectangleShape mSeparatorBetweenInputAndOutputArea;
	TerminalSharedData mTerminalSharedData;
};

}

#endif