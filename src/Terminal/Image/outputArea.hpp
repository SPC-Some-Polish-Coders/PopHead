#ifndef POPHEAD_TERMINAL_IMAGE_OUTPUTAREA_H_
#define POPHEAD_TERMINAL_IMAGE_OUTPUTAREA_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include <string>

namespace ph {

class GameData;

struct OutputLine
{
	std::string mText;
	sf::Color mColor;
};

class OutputArea : public sf::Drawable
{
public:
	void init(GameData* gameData);

	void pushOutputLine(const OutputLine&);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void move(sf::Vector2f offset);

private:
	std::vector<sf::Text> mTexts;
	std::deque<OutputLine> mOutputLines;
};

}

#endif // !POPHEAD_TERMINAL_IMAGE_OUTPUTAREA_H_