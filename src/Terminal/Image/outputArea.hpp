#ifndef POPHEAD_TERMINAL_IMAGE_OUTPUTAREA_H_
#define POPHEAD_TERMINAL_IMAGE_OUTPUTAREA_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include <string>

namespace ph {

class GameData;

class OutputArea : public sf::Drawable
{
public:
	void init(GameData* gameData);

	void pushOutputText(const std::string& text);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void move(sf::Vector2f offset);

private:
	std::vector<sf::Text> mOutputLines;
	std::deque<std::string> mContentOfLines;
};

}

#endif // !POPHEAD_TERMINAL_IMAGE_OUTPUTAREA_H_