#pragma once

#include "cast.hpp"
#include <string>
#include <vector>

namespace ph {

class Xml 
{
public:
	void loadFromFile(std::string filePath);

	Xml getChild(const std::string& name) const;

	std::vector<Xml> getChildren(const std::string& name) const;

	bool hasAttribute(const std::string& name) const;

	Xml getAttribute(const std::string& name) const;

	std::string toString() const;

	bool toBool() const;

	int toInt() const;

	unsigned toUnsigned() const;

	char toChar() const;

	unsigned char toUnsignedChar() const;
	
	float toFloat() const;

private:
	bool isSelfClosingTag(std::size_t openingTagEndPosition) const;

	bool isClosingTag(std::size_t tagNamePosition) const;

	bool isEmptyAttributeValue(std::size_t onePositionAfterAttributeValueOpeningQuote) const;

	std::size_t findEndOfTagAttributes(std::size_t offset = 0) const;

	inline static const std::string whitespaceCharacters = " \n\t\v\f\r";
	std::string mContent;
};

}
