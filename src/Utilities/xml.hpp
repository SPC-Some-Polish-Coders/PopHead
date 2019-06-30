#pragma once

#include "cast.hpp"
#include <string>
#include <vector>

namespace ph {

class Xml 
{
public:
	void loadFromFile(const std::string& filePath);

	Xml getChild(std::string name) const;

	std::vector<Xml> getChildren(std::string name) const;

	bool hasAttribute(std::string name) const;

	Xml getAttribute(std::string name) const;

	std::string toString() const;

	bool toBool() const { return Cast::toBool(mContent); }

	int toInt() const { return std::stoi(mContent); }

	unsigned toUnsigned() const { return Cast::toUnsigned(mContent); }

	float toFloat() const { return std::stof(mContent); }

private:
	bool isSelfClosingTag(std::size_t openingTagEndPosition) const
	{ return mContent[openingTagEndPosition - 1] == '/'; }

	bool isEmptyAttributeValue(std::size_t onePositionAfterAttributeValueOpeningQuote) const
	{ return mContent[onePositionAfterAttributeValueOpeningQuote] == '\"'; }

	std::size_t findEndOfCurrentTagAttributes() const
	{ return mContent.find('>'); }

	std::string mContent;
};

}
