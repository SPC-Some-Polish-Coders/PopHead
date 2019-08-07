#include "xml.hpp"
#include "Logs/logs.hpp"
#include <fstream>

namespace ph {

void Xml::loadFromFile(std::string filePath)
{
	mContent.clear();
	filePath.insert(0, "resources/");
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

std::optional<Xml> Xml::getChild(std::string name) const
{
	PH_ASSERT(!name.empty(), "child name cannot be empty");
	std::size_t begin = findEndOfTagAttributes();
	PH_ASSERT(begin != std::string::npos, "missing closing angle bracket");
	if (isSelfClosingTag(begin))
		return std::nullopt;
	while (true) {
		begin = mContent.find('<', begin + 1);
		if (begin == std::string::npos)
			return std::nullopt;
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

std::vector<Xml> Xml::getChildren(std::string name) const
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

bool Xml::hasAttribute(std::string name) const
{
	PH_ASSERT(!name.empty(), "attribute name cannot be empty");
	std::size_t endOfTagAttributes = findEndOfTagAttributes();
	if (endOfTagAttributes == std::string::npos)
		PH_EXCEPTION("missing closing angle bracket in child opening tag");
	if (isSelfClosingTag(endOfTagAttributes))
		--endOfTagAttributes;
	std::size_t begin = 0;
	while (true) {
		begin = mContent.find_first_of(whitespaceCharacters, begin + 1);
		if (begin == std::string::npos || begin > endOfTagAttributes) {
			PH_LOG_INFO("Xml hasAttribute(): false");
			return false;
		}
		begin = mContent.find_first_not_of(whitespaceCharacters, begin + 1);
		PH_ASSERT(begin != std::string::npos && begin <= endOfTagAttributes, "it should be farthest at endOfTagAttributes");
		if (begin == endOfTagAttributes) {
			PH_LOG_INFO("Xml hasAttribute(): false");
			return false;
		}
		std::size_t end = mContent.find_first_of("=" + whitespaceCharacters, begin + 1);
		if (end == std::string::npos || end > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value");
		if (mContent.compare(begin, end - begin, name) == 0) {
			PH_LOG_INFO("Xml hasAttribute(): true");
			return true;
		}
		begin = mContent.find('\"', end + 1);
		if (begin == std::string::npos || begin > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value opening quote");
		begin = mContent.find('\"', begin + 1);
		if (begin == std::string::npos || begin > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value closing quote");
	}
}

Xml Xml::getAttribute(std::string name) const
{
	PH_ASSERT(!name.empty(), "attribute name cannot be empty");
	std::size_t endOfTagAttributes = findEndOfTagAttributes();
	if (endOfTagAttributes == std::string::npos)
		PH_EXCEPTION("missing closing angle bracket in child opening tag");
	if (isSelfClosingTag(endOfTagAttributes))
		--endOfTagAttributes;
	std::size_t begin = 0;
	while (true) {
		begin = mContent.find_first_of(whitespaceCharacters, begin + 1);
		if(begin == std::string::npos || begin > endOfTagAttributes)
			PH_EXCEPTION("attribute name cannot be found");
		begin = mContent.find_first_not_of(whitespaceCharacters, begin + 1);
		PH_ASSERT(begin != std::string::npos && begin <= endOfTagAttributes, "it should be farthest at endOfTagAttributes");
		if (begin == endOfTagAttributes)
			PH_EXCEPTION("attribute name cannot be found");
		std::size_t end = mContent.find_first_of("=" + whitespaceCharacters, begin + 1);
		if(end == std::string::npos || end > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value");
		if (mContent.compare(begin, end - begin, name) == 0) {
			begin = mContent.find('\"', end + 1);
			if (begin == std::string::npos || begin > endOfTagAttributes)
				PH_EXCEPTION("missing attribute value opening quote");
			++begin;
			end = mContent.find('\"', begin);
			if (end == std::string::npos || end > endOfTagAttributes)
				PH_EXCEPTION("missing attribute value closing quote");
			Xml xml;
			xml.mContent = mContent.substr(begin, end - begin);
			PH_LOG_INFO("Xml getAttribute(): " + xml.mContent);
			return xml;
		}
		begin = mContent.find('\"', end + 1);
		if(begin == std::string::npos || begin > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value opening quote");
		begin = mContent.find('\"', begin + 1);
		if (begin == std::string::npos || begin > endOfTagAttributes)
			PH_EXCEPTION("missing attribute value closing quote");
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

}