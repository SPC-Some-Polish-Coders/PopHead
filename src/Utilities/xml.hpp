#ifndef POPHEAD_UTILITIES_XML_H_
#define POPHEAD_UTILITIES_XML_H_

#include "cast.hpp"
#include <string>
#include <vector>

namespace PopHead {
namespace Utilities {

class Xml {
public:
	void loadFromFile(const std::string& filename);

	Xml getChild(std::string name) const;

	std::vector<Xml> getChildren(std::string name) const;

	Xml getAttribute(std::string name) const;

	std::string toString() const;

	unsigned toInt() const { return std::stoi(content); }

	unsigned toUnsigned() const { return Cast::toUnsigned(content); }

private:
	bool isSelfClosingTag(std::size_t openingTagEndPosition) const
	{ return content[openingTagEndPosition - 1] == '/'; }

	bool isEmptyAttributeValue(std::size_t onePositionAfterAttributeValueOpeningQuote) const
	{ return content[onePositionAfterAttributeValueOpeningQuote] == '\"'; }

	std::size_t findEndOfCurrentTagAttributes() const
	{ return content.find('>'); }

	std::string content;
};

}}

#endif // !POPHEAD_UTILITIES_XML_H_
