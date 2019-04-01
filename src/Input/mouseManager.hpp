#ifndef POPHEAD_INPUT_MOUSEMANAGER_H_
#define POPHEAD_INPUT_MOUSEMANAGER_H_

#include <SFML/Graphics.hpp>

namespace PopHead {
namespace Input {

class MouseManager
{
public:
    MouseEvent();

    sf::Event::MouseButtonEvent getMouseButtonEvent() const;
    sf::Vector2f getMousePosition() const;
    bool isClicked( const sf::Rect<int>& ) const;
    bool isClicked( const sf::Sprite& ) const;

private:
    sf::Vector2f previousMousePos;
};

}}

#endif // !POPHEAD_INPUT_MOUSEMANAGER_H_
