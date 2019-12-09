#include "xml.hpp"
#include "Logs/logs.hpp"
#include <fstream>

namespace ph {

void Xml::loadFromFile(std::string filePath)
{
	mContent.clear();
	std::ifstream ifs(filePath);
	if (!ifs.is_open())
		PH_EXCEPTION("cannot open file: " + filePath);
	std::string line;
	if (!std::getline(ifs, line))
		PH_EXCEPTION("given xml file is empty or something bad happened (" + filePath + ")");
	// NOTE: Delete prolog but keep '?>' for implementation purpose
	const std::size_t begin = line.find("?>");
	if (begin == std::string::npos)
		line.insert(0, "?>");
	else
		line.erase(0, begin);
	mContent += line;
	while (std::getline(ifs, line))
		mContent += line;
	PH_LOG_INFO("Xml loadFromFile(): " + mContent);
}

Xml Xml::getChild(const std::string& name) const
{
	PH_ASSERT(!name.empty(), "child name cannot be empty");
	std::size_t begin = findEndOfTagAttributes();
	PH_ASSERT(begin != std::string::npos, "missing closing angle bracket");
	if (isSelfClosingTag(begin))
		PH_EXCEPTION("current tag cannot have children");
	while (true) {
		begin = mContent.find('<', begin + 1);
		if (begin == std::string::npos)
			PH_EXCEPTION("cannot find child");
		++begin;
		std::size_t end = mContent.find_first_of(whitespaceCharacters + ">", begin + 1);
		if (end == std::string::npos)
			PH_EXCEPTION("missing angle bracket in child opening tag");
		if (isSelfClosingTag(end)) {
			if (mContent.compare(begin, end - begin - 1, name) == 0) {
				Xml xml;
				xml.mContent = mContent.substr(begin, end - begin + 1);
				PH_LOG_INFO("Xml getChild(): " + xml.mContent);
				return xml;
			}
			else
				begin = end;
		}
		else if (mContent.compare(begin, end - begin, name) == 0) {
			if (mContent[end] != '>') {
				end = findEndOfTagAttributes(end + 1);
				if (end == std::string::npos)
					PH_EXCEPTION("missing closing angle bracket in child opening tag");
			}
			if (isSelfClosingTag(end)) {
				Xml xml;
				xml.mContent = mContent.substr(begin, end - begin + 1);
				PH_LOG_INFO("Xml getChild(): " + xml.mContent);
				return xml;
			}
			unsigned count = 0;
			while (true) {
				end = mContent.find(name, end + 2);
				if (end == std::string::npos)
					PH_EXCEPTION("missing child closing tag");
				if (isClosingTag(end) && count == 0) {
					Xml xml;
					xml.mContent = mContent.substr(begin, end - begin - 2);
					PH_LOG_INFO("Xml getChild(): " + xml.mContent);
					return xml;
				}
				else if (isClosingTag(end)) {
					--count;
					end += name.size();
				}
				else if (mContent[end - 1] == '<') {
					end = findEndOfTagAttributes(end + name.size());
					if (end == std::string::npos)
						PH_EXCEPTION("missing closing angle bracket in child opening tag");
					if (!isSelfClosingTag(end))
						++count;
				}
				else
					end += name.size() - 1;
			}
		}
		else {
			/* 
				NOTE:
				It is child which is not equel to given name paramiter.
				So we need to find his ending tag to skip him and his children.
			*/
			const std::string childName = mContent.substr(begin, end - begin);
			begin = end;
			if (mContent[begin] != '>') {
				begin = findEndOfTagAttributes(begin + 1);
				if (begin == std::string::npos)
					PH_EXCEPTION("missing closing angle bracket in child opening tag");
			}
			if (isSelfClosingTag(begin))
				continue;
			unsigned count = 0;
			while (true) {
				begin = mContent.find(childName, begin + 2);
				if (begin == std::string::npos)
					PH_EXCEPTION("missing child closing tag");
				if (isClosingTag(begin) && count == 0) {
					begin += childName.size();
					break;
				}
				else if (isClosingTag(begin)) {
					--count;
					begin += childName.size();
				}
				else if (mContent[begin - 1] == '<') {
					begin = findEndOfTagAttributes(begin + childName.size());
					if (begin == std::string::npos)
						PH_EXCEPTION("missing closing angle bracket in child opening tag");
					if (!isSelfClosingTag(begin))
						++count;
				}
				else
					begin += childName.size() - 1;
			}
		}
	}
}

std::vector<Xml> Xml::getChildren(const std::string& name) const
{
	PH_ASSERT(!name.empty(), "child name cannot be empty");
	std::size_t begin = findEndOfTagAttributes();
	PH_ASSERT(begin != std::string::npos, "missing closing angle bracket");
	if (isSelfClosingTag(begin))
		return std::vector<Xml>();
	std::vector<Xml> children;
	while ((begin = mContent.find('<', begin + 1)) != std::string::npos) {
		++begin;
		std::size_t end = mContent.find_first_of(whitespaceCharacters + " >", begin + 1);
		if (end == std::string::npos)
			PH_EXCEPTION("missing angle bracket in child opening tag");
		if (isSelfClosingTag(end)) {
			if (mContent.compare(begin, end - begin - 1, name) == 0) {
				Xml xml;
				xml.mContent = mContent.substr(begin, end - begin + 1);
				PH_LOG_INFO("Xml getChildren(): " + xml.mContent);
				children.push_back(xml);
				begin = end;
				continue;
			}
			else
				begin = end;
		}
		else if (mContent.compare(begin, end - begin, name) == 0) {
			if (mContent[end] != '>') {
				end = findEndOfTagAttributes(end + 1);
				if (end == std::string::npos)
					PH_EXCEPTION("missing closing angle bracket in child opening tag");
				if (isSelfClosingTag(end)) {
					Xml xml;
					xml.mContent = mContent.substr(begin, end - begin + 1);
					PH_LOG_INFO("Xml getChildren(): " + xml.mContent);
					children.push_back(xml);
					begin = end;
					continue;
				}
			}
			unsigned count = 0;
			while (true) {
				end = mContent.find(name, end + 2);
				if (end == std::string::npos)
					PH_EXCEPTION("missing child closing tag");
				if (isClosingTag(end) && count == 0) {
					Xml xml;
					xml.mContent = mContent.substr(begin, end - begin - 2);
					PH_LOG_INFO("Xml getChildren(): " + xml.mContent);
					children.push_back(xml);
					begin = end + name.size();
					break;
				}
				else if (isClosingTag(end)) {
					--count;
					end += name.size();
				}
				else if (mContent[end - 1] == '<') {
					end = findEndOfTagAttributes(end + name.size());
					if (end == std::string::npos)
						PH_EXCEPTION("missing closing angle bracket in child opening tag");
					if (!isSelfClosingTag(end))
						++count;
				}
				else
					end += name.size() - 1;
			}
		}
		else {
			/*
				NOTE:
				It is child which is not equel to given name paramiter.
				So we need to find his ending tag to skip him and his children.
			*/
			const std::string childName = mContent.substr(begin, end - begin);
			begin = end;
			if (mContent[begin] != '>') {
				begin = findEndOfTagAttributes(begin + 1);
				if (begin == std::string::npos)
					PH_EXCEPTION("missing closing angle bracket in child opening tag");
				if (isSelfClosingTag(begin))
					continue;
			}
			unsigned count = 0;
			while (true) {
				begin = mContent.find(childName, begin + 2);
				if (begin == std::string::npos)
					PH_EXCEPTION("missing child closing tag");
				if (isClosingTag(begin) && count == 0) {
					begin += childName.size();
					break;
				}
				else if (isClosingTag(begin)) {
					--count;
					begin += childName.size();
				}
				else if (mContent[begin - 1] == '<') {
					begin = findEndOfTagAttributes(begin + childName.size());
					if (begin == std::string::npos)
						PH_EXCEPTION("missing closing angle bracket in child opening tag");
					if (!isSelfClosingTag(begin))
						++count;
				}
				else
					begin += childName.size() - 1;
			}
		}
	}
	return children;
}

bool Xml::hasAttribute(const std::string& name) const
{
	PH_ASSERT(!name.empty(), "attribute name cannot be empty");
	std::size_t endOfTagAttributes = findEndOfTagAttributes();
	if (endOfTagAttributes == std::string::npos)
		PH_EXCEPTION("missing closing angle bracket in opening tag");
	if (isSelfClosingTag(endOfTagAttributes))
		--endOfTagAttributes;
	std::size_t begin = mContent.find_first_of(whitespaceCharacters, 1);
	if (begin > endOfTagAttributes) {
		PH_LOG_INFO("Xml hasAttribute(): false");
		return false;
	}
	while (true) {
		begin = mContent.find_first_not_of(whitespaceCharacters, begin + 1);
		if (begin == endOfTagAttributes) {
			PH_LOG_INFO("Xml hasAttribute(): false");
			return false;
		}
		std::size_t end = mContent.find_first_of("=" + whitespaceCharacters, begin + 1);
		if (end > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value");
		if (mContent.compare(begin, end - begin, name) == 0) {
			PH_LOG_INFO("Xml hasAttribute(): true");
			return true;
		}
		begin = mContent.find('\"', end + 1);
		if (begin > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value opening quote");
		begin = mContent.find('\"', begin + 1);
		if (begin > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value closing quote");
	}
}

Xml Xml::getAttribute(const std::string& name) const
{
	PH_ASSERT(!name.empty(), "attribute name cannot be empty");
	std::size_t endOfTagAttributes = findEndOfTagAttributes();
	if (endOfTagAttributes == std::string::npos)
		PH_EXCEPTION("missing closing angle bracket in opening tag");
	if (isSelfClosingTag(endOfTagAttributes))
		--endOfTagAttributes;
	std::size_t begin = mContent.find_first_of(whitespaceCharacters, 1);
	if (begin > endOfTagAttributes)
		PH_EXCEPTION("attribute name cannot be found");
	while (true) {
		begin = mContent.find_first_not_of(whitespaceCharacters, begin + 1);
		if (begin == endOfTagAttributes)
		PH_EXCEPTION("attribute name cannot be found");
		std::size_t end = mContent.find_first_of("=" + whitespaceCharacters, begin + 1);
		if(end > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value");
		const bool foundName = mContent.compare(begin, end - begin, name) == 0;
		begin = mContent.find('\"', end + 1);
		if (begin > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value opening quote");
		++begin;
		end = mContent.find('\"', begin);
		if (end > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value closing quote");
		if (foundName) {
			Xml xml;
			xml.mContent = mContent.substr(begin, end - begin);
			PH_LOG_INFO("Xml getAttribute(): " + xml.mContent);
			return xml;
		}
		begin = end;
	}
}

std::string Xml::toString() const
{
	std::size_t begin = findEndOfTagAttributes();
	if (begin == std::string::npos)
		return mContent;
	++begin;
	return mContent.substr(begin, mContent.size() - begin);
}

bool Xml::toBool() const
{
	return Cast::toBool(toString());
}

int Xml::toInt() const
{
	return std::stoi(toString());
}

unsigned Xml::toUnsigned() const
{
	return Cast::toUnsigned(toString());
}

char Xml::toChar() const
{
	return static_cast<char>(std::stoi(toString()));
}

unsigned char Xml::toUnsignedChar() const
{
	return static_cast<unsigned char>(Cast::toUnsigned(toString()));
}

float Xml::toFloat() const
{
	return std::stof(toString());
}

sf::Color Xml::toColor() const
{
	// TODO: Optimize and refactor this function:
	// - get rid of std::vector
	// - get rid of 2 if blocks which do almost the same
	// - can't we assume bracket pos?

	auto intoUint8 = [](const std::string& str) -> sf::Uint8
	{
		return static_cast<sf::Uint8>(std::stoi(str));
	};

	if(mContent.find("rgba") != std::string::npos)
	{
		auto bracketPos = mContent.find('(');

		std::vector<size_t> commas;
		commas.push_back(mContent.find(','));
		commas.push_back(mContent.find(',', commas[0] + 1));
		commas.push_back(mContent.find(',', commas[1] + 1));

		std::vector<sf::Uint8> values = { 
			intoUint8(mContent.substr(bracketPos + 1, commas[0] - bracketPos - 1)),
			intoUint8(mContent.substr(commas[0] + 1, commas[1] - commas[0])),
			intoUint8(mContent.substr(commas[1] + 1, commas[2] - commas[1])),
			intoUint8(mContent.substr(commas[2] + 1))
		};

		return sf::Color(values[0], values[1], values[2], values[3]);
	}
	else if(mContent.find("rgb") != std::string::npos)
	{
		auto bracketPos = mContent.find('(');
			
		std::vector<size_t> commas;
		commas.push_back(mContent.find(','));
		commas.push_back(mContent.find(',', commas[0] + 1));

		std::vector<sf::Uint8> values = { 
			intoUint8(mContent.substr(bracketPos + 1, commas[0] - bracketPos - 1)), 
			intoUint8(mContent.substr(commas[0] + 1, commas[1] - commas[0])),
			intoUint8(mContent.substr(commas[1] + 1))
		};

		return sf::Color(values[0], values[1], values[2]);
	}

	PH_EXIT_GAME("Could not cast to color!");
}

bool Xml::isSelfClosingTag(std::size_t openingTagEndPosition) const
{
	return mContent[openingTagEndPosition - 1] == '/';
}

bool Xml::isClosingTag(std::size_t tagNamePosition) const
{
	return mContent[tagNamePosition - 2] == '<' && mContent[tagNamePosition - 1] == '/';
}

bool Xml::isEmptyAttributeValue(std::size_t onePositionAfterAttributeValueOpeningQuote) const
{
	return mContent[onePositionAfterAttributeValueOpeningQuote] == '\"';
}

std::size_t Xml::findEndOfTagAttributes(std::size_t offset) const
{
	return mContent.find('>', offset);
}

}
