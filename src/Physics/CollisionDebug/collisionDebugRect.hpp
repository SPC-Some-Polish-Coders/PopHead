
// TEMPORARILY WE DON'T SUPPORT COLLISION DEBUG

//#pragma once
//
//#include "SFML/Graphics.hpp"
//
//namespace ph {
//
//class CollisionDebugRect : public sf::Drawable
//{
//public:
//    CollisionDebugRect(sf::FloatRect rect);
//
//    void move(sf::Vector2f velocity) { mShape.move(velocity); }
//    void setPosition(sf::Vector2f position) { mShape.setPosition(position); }
//
//    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
//	void setColor(sf::Color color);
//
//private:
//    mutable sf::RectangleShape mShape;
//};
//
//}
