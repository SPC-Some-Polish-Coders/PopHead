#include "xml.hpp"
#include "Logs/logs.hpp"
#include <fstream>

namespace ph {

bool Xml::loadFromFile(const std::string& filePath)
{
	mContent.clear();
	std::ifstream ifs(filePath);
	if(!ifs.is_open())
		return false;
	std::string line;
	if(!std::getline(ifs, line))
		return false;
	// NOTE: Delete prolog but keep '?>' for implementation purpose
	const std::size_t begin = line.find("?>");
	if(begin == std::string::npos)
		line.insert(0, "?>");
	else
		line.erase(0, begin);
	mContent += line;
	while(std::getline(ifs, line))
		mContent += line;
	return true;
}

std::optional<Xml> Xml::getChild(const std::string& name) const
{
	PH_ASSERT_CRITICAL(!name.empty(), "child name cannot be empty");
	std::size_t begin = findEndOfTagAttributes();
	PH_ASSERT_CRITICAL(begin != std::string::npos, "missing closing angle bracket");
	if(isSelfClosingTag(begin))
		return std::nullopt;
	while(true) {
		begin = mContent.find('<', begin + 1);
		if(begin == std::string::npos)
			return std::nullopt;
		++begin;
		std::size_t end = mContent.find_first_of(whitespaceCharacters + ">", begin + 1);
		if(end == std::string::npos)
			PH_EXIT_GAME("missing angle bracket in child opening tag");
		if(isSelfClosingTag(end)) {
			if(mContent.compare(begin, end - begin - 1, name) == 0) {
				Xml xml;
				xml.mContent = mContent.substr(begin, end - begin + 1);
				return xml;
			}
			else
				begin = end;
		}
		else if(mContent.compare(begin, end - begin, name) == 0) {
			if(mContent[end] != '>') {
				end = findEndOfTagAttributes(end + 1);
				if(end == std::string::npos)
					PH_EXIT_GAME("missing closing angle bracket in child opening tag");
			}
			if(isSelfClosingTag(end)) {
				Xml xml;
				xml.mContent = mContent.substr(begin, end - begin + 1);
				PH_LOG_INFO("Xml getChild(): " + xml.mContent);
				return xml;
			}
			unsigned count = 0;
			while(true) {
				end = mContent.find(name, end + 2);
				if(end == std::string::npos)
					PH_EXIT_GAME("missing child closing tag");
				if(isClosingTag(end) && count == 0) {
					Xml xml;
					xml.mContent = mContent.substr(begin, end - begin - 2);
					return xml;
				}
				else if(isClosingTag(end)) {
					--count;
					end += name.size();
				}
				else if(mContent[end - 1] == '<') {
					end = findEndOfTagAttributes(end + name.size());
					if(end == std::string::npos)
						PH_EXIT_GAME("missing closing angle bracket in child opening tag");
					if(!isSelfClosingTag(end))
						++count;
				}
				else
					end += name.size() - 1;
			}
		}
		else {
			/*
				NOTE:
				It is child which is not equel to given name parameter.
				So we need to find his ending tag to skip him and his children.
			*/
			const std::string childName = mContent.substr(begin, end - begin);
			begin = end;
			if(mContent[begin] != '>') {
				begin = findEndOfTagAttributes(begin + 1);
				if(begin == std::string::npos)
					PH_EXIT_GAME("missing closing angle bracket in child opening tag");
			}
			if(isSelfClosingTag(begin))
				continue;
			unsigned count = 0;
			while(true) {
				begin = mContent.find(childName, begin + 2);
				if(begin == std::string::npos)
					PH_EXIT_GAME("missing child closing tag");
				if(isClosingTag(begin) && count == 0) {
					begin += childName.size();
					break;
				}
				else if(isClosingTag(begin)) {
					--count;
					begin += childName.size();
				}
				else if(mContent[begin - 1] == '<') {
					begin = findEndOfTagAttributes(begin + childName.size());
					if(begin == std::string::npos)
						PH_EXIT_GAME("missing closing angle bracket in child opening tag");
					if(!isSelfClosingTag(begin))
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
	PH_ASSERT_CRITICAL(!name.empty(), "child name cannot be empty");
	std::size_t begin = findEndOfTagAttributes();
	PH_ASSERT_CRITICAL(begin != std::string::npos, "missing closing angle bracket");
	if(isSelfClosingTag(begin))
		return std::vector<Xml>();
	std::vector<Xml> children;
	while((begin = mContent.find('<', begin + 1)) != std::string::npos) {
		++begin;
		std::size_t end = mContent.find_first_of(whitespaceCharacters + " >", begin + 1);
		if(end == std::string::npos)
			PH_EXIT_GAME("missing angle bracket in child opening tag");
		if(isSelfClosingTag(end)) {
			if(mContent.compare(begin, end - begin - 1, name) == 0) {
				Xml xml;
				xml.mContent = mContent.substr(begin, end - begin + 1);
				children.push_back(xml);
				begin = end;
				continue;
			}
			else
				begin = end;
		}
		else if(mContent.compare(begin, end - begin, name) == 0) {
			if(mContent[end] != '>') {
				end = findEndOfTagAttributes(end + 1);
				if(end == std::string::npos)
					PH_EXIT_GAME("missing closing angle bracket in child opening tag");
				if(isSelfClosingTag(end)) {
					Xml xml;
					xml.mContent = mContent.substr(begin, end - begin + 1);
					children.push_back(xml);
					begin = end;
					continue;
				}
			}
			unsigned count = 0;
			while(true) {
				end = mContent.find(name, end + 2);
				if(end == std::string::npos)
					PH_EXIT_GAME("missing child closing tag");
				if(isClosingTag(end) && count == 0) {
					Xml xml;
					xml.mContent = mContent.substr(begin, end - begin - 2);
					children.push_back(xml);
					begin = end + name.size();
					break;
				}
				else if(isClosingTag(end)) {
					--count;
					end += name.size();
				}
				else if(mContent[end - 1] == '<') {
					end = findEndOfTagAttributes(end + name.size());
					if(end == std::string::npos)
						PH_EXIT_GAME("missing closing angle bracket in child opening tag");
					if(!isSelfClosingTag(end))
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
			if(mContent[begin] != '>') {
				begin = findEndOfTagAttributes(begin + 1);
				if(begin == std::string::npos)
					PH_EXIT_GAME("missing closing angle bracket in child opening tag");
				if(isSelfClosingTag(begin))
					continue;
			}
			unsigned count = 0;
			while(true) {
				begin = mContent.find(childName, begin + 2);
				if(begin == std::string::npos)
					PH_EXIT_GAME("missing child closing tag");
				if(isClosingTag(begin) && count == 0) {
					begin += childName.size();
					break;
				}
				else if(isClosingTag(begin)) {
					--count;
					begin += childName.size();
				}
				else if(mContent[begin - 1] == '<') {
					begin = findEndOfTagAttributes(begin + childName.size());
					if(begin == std::string::npos)
						PH_EXIT_GAME("missing closing angle bracket in child opening tag");
					if(!isSelfClosingTag(begin))
						++count;
				}
				else
					begin += childName.size() - 1;
			}
		}
	}
	return children;
}

std::optional<Xml> Xml::getAttribute(const std::string& name) const
{
	PH_ASSERT_CRITICAL(!name.empty(), "attribute name cannot be empty");
	std::size_t endOfTagAttributes = findEndOfTagAttributes();
	if(endOfTagAttributes == std::string::npos)
		PH_EXIT_GAME("missing closing angle bracket in opening tag");
	if(isSelfClosingTag(endOfTagAttributes))
		--endOfTagAttributes;
	std::size_t begin = mContent.find_first_of(whitespaceCharacters, 1);
	if(begin > endOfTagAttributes)
		return std::nullopt;
	while(true) {
		begin = mContent.find_first_not_of(whitespaceCharacters, begin + 1);
		if(begin == endOfTagAttributes)
			return std::nullopt;
		std::size_t end = mContent.find_first_of("=" + whitespaceCharacters, begin + 1);
		if(end > endOfTagAttributes)
			PH_EXIT_GAME("missing attribute value");
		const bool foundName = mContent.compare(begin, end - begin, name) == 0;
		begin = mContent.find('\"', end + 1);
		if(begin > endOfTagAttributes)
			PH_EXIT_GAME("missing attribute value opening quote");
		++begin;
		end = mContent.find('\"', begin);
		if(end > endOfTagAttributes)
			PH_EXIT_GAME("missing attribute value closing quote");
		if(foundName) {
			Xml xml;
			xml.mContent = mContent.substr(begin, end - begin);
			return xml;
		}
		begin = end;
	}
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

std::string Xml::toString() const
{
	std::size_t begin = findEndOfTagAttributes();
	if(begin == std::string::npos)
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
	auto intoUint8 = [](const std::string& str) -> sf::Uint8 {
		return static_cast<sf::Uint8>(std::stoi(str));
	};

	if(mContent.find("rgba") != std::string::npos)
	{
		size_t bracketPos = 4;

		size_t commas[3];
		commas[0] = mContent.find(',');
		commas[1] = mContent.find(',', commas[0] + 1);
		commas[2] = mContent.find(',', commas[1] + 1);

		sf::Uint8 values[] = {
			intoUint8(mContent.substr(bracketPos + 1, commas[0] - bracketPos - 1)),
			intoUint8(mContent.substr(commas[0] + 1, commas[1] - commas[0])),
			intoUint8(mContent.substr(commas[1] + 1, commas[2] - commas[1])),
			intoUint8(mContent.substr(commas[2] + 1))
		};

		return sf::Color(values[0], values[1], values[2], values[3]);
	}
	else if(mContent.find("rgb") != std::string::npos)
	{
		size_t bracketPos = 3;

		size_t commas[2];
		commas[0] = mContent.find(',');
		commas[1] = mContent.find(',', commas[0] + 1);

		sf::Uint8 values[] = {
			intoUint8(mContent.substr(bracketPos + 1, commas[0] - bracketPos - 1)),
			intoUint8(mContent.substr(commas[0] + 1, commas[1] - commas[0])),
			intoUint8(mContent.substr(commas[1] + 1))
		};

		return sf::Color(values[0], values[1], values[2]);
	}
	else if(mContent == "black")
		return sf::Color::Black;
	else if(mContent == "white")
		return sf::Color::White;
	else if(mContent == "red")
		return sf::Color::Red;
	else if(mContent == "green")
		return sf::Color::Green;
	else if(mContent == "blue")
		return sf::Color::Blue;
	else if(mContent == "yellow")
		return sf::Color::Yellow;
	else if(mContent == "magenta")
		return sf::Color::Magenta;
	else if(mContent == "cyan")
		return sf::Color::Cyan;
	else if(mContent == "transparent")
		return sf::Color::Transparent;
	else
		return sf::Color::Black;
}

sf::Vector2f Xml::toVector2f() const
{
	size_t comma = mContent.find(',');
	const std::string x = mContent.substr(0, comma);
	const std::string y = mContent.substr(comma + 1, std::string::npos);
	return sf::Vector2f(std::stof(x), std::stof(y));
}

}

