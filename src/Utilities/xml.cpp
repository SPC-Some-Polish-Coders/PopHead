#include "xml.hpp"
#include "Utilities/debug.hpp"
#include <fstream>

using PopHead::Utilities::Xml;

void Xml::loadFromFile(const std::string& filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		PH_EXCEPTION("cannot open file: " + filename);
	std::string temp;
	while (std::getline(ifs, temp))
		content += temp;
	// Delete prolog but keep '>' for implementation purpose
	const std::size_t begin = content.find("?>");
	if (begin != std::string::npos)
		content.erase(0, begin + std::strlen("?>") - 1);
	else 
		content.insert(0, ">"); // TODO: It has terrible performance probably
	PH_LOG(LogType::Info, std::string("Xml loadFromFile(): ") + content);
}

Xml Xml::getChild(std::string name) const
{
	PH_ASSERT(!name.empty(), "child name cannot be empty");
	name.insert(0, "<");

	std::size_t begin = findEndOfCurrentTagAttributes();
	if (begin == std::string::npos)
		PH_EXCEPTION("missing angle bracket");
	else if (isSelfClosingTag(begin))
		PH_EXCEPTION("current tag cannot have children");

	begin = content.find(name, begin + 1);
	if (begin == std::string::npos)
		PH_EXCEPTION("cannot find child");
	++begin; // WARNING: Don't use += name.size() here if you want to keep tag name

	std::size_t end = content.find('>', begin);
	if (end == std::string::npos)
		PH_EXCEPTION("missing angle bracket in child opening tag");

	if (isSelfClosingTag(end))
		++end;
	else {
		name.insert(1, "/");
		name.push_back('>');

		end = content.find(name, end + 1);
		if (end == std::string::npos)
			PH_EXCEPTION("missing closing tag in child");
	}

	Xml xml;
	xml.content = content.substr(begin, end - begin);
	PH_LOG(LogType::Info, "Xml getChild(): " + xml.content);
	return xml;
}

std::vector<Xml> Xml::getChildren(std::string name) const
{
	PH_ASSERT(!name.empty(), "child name cannot be empty");
	name.insert(0, "<");

	std::size_t begin = findEndOfCurrentTagAttributes();
	if (begin == std::string::npos)
		PH_EXCEPTION("missing angle bracket");
	else if (isSelfClosingTag(begin))
		return std::vector<Xml>();

	begin = content.find(name, begin + 1);
	if (begin == std::string::npos)
		return std::vector<Xml>();
	++begin; // WARNING: Don't use += name.size() here if you want to keep tag name

	std::size_t end = content.find('>', begin);
	if (end == std::string::npos)
		PH_EXCEPTION("missing angle bracket in child opening tag");

	std::string endingTag = name;
	endingTag.insert(1, "/");
	endingTag.push_back('>');

	std::vector<Xml> children;
	if (isSelfClosingTag(end))
		begin = content.find(name, end + 1);
	else {
		end = content.find(endingTag, end + 1);
		if (end == std::string::npos)
			PH_EXCEPTION("missing closing tag in child");

		Xml xml;
		xml.content = content.substr(begin, end - begin);
		PH_LOG(LogType::Info, "Xml getChildren(): " + xml.content);
		children.push_back(xml);
		begin = content.find(name, end + endingTag.size());
	}
	while (begin != std::string::npos) {
		++begin; // WARNING: Don't use += name.size() here if you want to keep tag name

		end = content.find('>', begin);
		if (end == std::string::npos)
			PH_EXCEPTION("missing angle bracket in child opening tag");
		if (isSelfClosingTag(end))
			begin = content.find(name, end + 1);
		else {
			end = content.find(endingTag, end + 1);
			if (end == std::string::npos)
				PH_EXCEPTION("missing closing tag in child");

			Xml xml;
			xml.content = content.substr(begin, end - begin);
			PH_LOG(LogType::Info, "Xml getChildren(): " + xml.content);
			children.push_back(xml);
			begin = content.find(name, end + endingTag.size());
		}
	}
	return children;
}

Xml Xml::getAttribute(std::string name) const
{
	PH_ASSERT(!name.empty(), "attribute name cannot be empty");
	/*
		NOTE:
		This partly protects against situation when attribute name will
		be in part of other attribute name or in attribute value

		WARNING: It fails with spaces: <foo bar  =    ""
		TODO: Delete unnecessary spaces in loadFromFile()?
	*/
	name.insert(0, " ");
	name.push_back('=');
	name.push_back('\"');

	// TODO: Improve performance by doing reverse find from end?
	std::size_t begin = content.find(name);
	if (begin == std::string::npos)
		PH_EXCEPTION("attribute name cannot be found");
	begin += name.size();

	std::size_t end = content.find('>');
	if (end == std::string::npos)
		PH_EXCEPTION("missing angle bracket in child opening tag");
	if (isSelfClosingTag(end))
		--end;
	if (begin >= end)
		PH_EXCEPTION("attribute name cannot be found");
	if (isEmptyAttributeValue(begin))
		return Xml();

	end = content.find('\"', begin + 1);
	if (end == std::string::npos)
		PH_EXCEPTION("missing closing quote");

	Xml xml;
	xml.content = content.substr(begin, end - begin);
	PH_LOG(LogType::Info, "Xml getAttribute(): " + xml.content);
	return xml;
}

std::string Xml::toString() const
{
	std::size_t begin = findEndOfCurrentTagAttributes();
	if (begin == std::string::npos)
		return content;
	++begin;
	return content.substr(begin, content.size() - begin);
}
