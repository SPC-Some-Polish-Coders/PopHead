#pragma once

#include "textWidget.hpp"

namespace ph {

class GameData;

class Interface : public Widget
{
public:
	Interface();

	Interface(GameData* data);

	void update(sf::Time delta) override;

	void draw() override;

	bool setContentPath(const std::string& path) override;

	void setPosition(const sf::Vector2f& pos) override;

	void addWidget(const std::string& name, Widget* ptr) override;

	void move(const sf::Vector2f& delta) override;

	sf::Vector2f getGlobalPosition() const override;
};

}
