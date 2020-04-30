#pragma once

#include "cast.hpp"
#include <string>
#include <vector>
#include <optional>
#include <SFML/Graphics/Color.hpp>

namespace ph {

class Xml 
{
public:
	bool loadFromFile(const std::string& filePath);

	std::optional<Xml> getChild(const std::string& name) const;
	std::vector<Xml> getChildren(const std::string& name) const;
	std::optional<Xml> getAttribute(const std::string& name) const;

	std::string toString() const;
	bool toBool() const;
	int toInt() const;
	unsigned toUnsigned() const;
	char toChar() const;
	unsigned char toUnsignedChar() const;
	float toFloat() const;
	sf::Color toColor() const;
	sf::Vector2f toVector2f() const;

private:
	bool isSelfClosingTag(std::size_t openingTagEndPosition) const;
	bool isClosingTag(std::size_t tagNamePosition) const;
	bool isEmptyAttributeValue(std::size_t onePositionAfterAttributeValueOpeningQuote) const;
	std::size_t findEndOfTagAttributes(std::size_t offset = 0) const;

private:
	std::string mContent;
};

}

