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
	const char* toCString() const;
	bool toBool() const;
	i32 toI32() const;
	u32 toU32() const;
	u16 toI16() const;
	u16 toU16() const;
	i8 toI8() const;
	u8 toU8() const;
	float toFloat() const;
	sf::Color toColor() const;
	Vec2 toVec2() const;

private:
	bool isSelfClosingTag(std::size_t openingTagEndPosition) const;
	bool isClosingTag(std::size_t tagNamePosition) const;
	bool isEmptyAttributeValue(std::size_t onePositionAfterAttributeValueOpeningQuote) const;
	std::size_t findEndOfTagAttributes(std::size_t offset = 0) const;

private:
	std::string mContent;
};

}

