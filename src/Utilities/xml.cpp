#include "xml.hpp"
#include "Utilities/debug.hpp"
#include <fstream>

namespace ph {

void Xml::loadFromFile(const std::string& filePath)
{
	mContent.clear();
	std::string fullFilePath = "resources/" + filePath;
	std::ifstream ifs(fullFilePath);
	if (!ifs.is_open())
		PH_EXCEPTION("cannot open file: " + fullFilePath);
	std::string temp;
	if (!std::getline(ifs, temp))
		PH_EXCEPTION("given xml file is empty or something bad happened (" + fullFilePath + ")");
	// Delete prolog but keep '?>' for implementation purpose
	const std::size_t begin = temp.find("?>");
	if (begin == std::string::npos)
		temp.insert(0, "?>");
	else
		temp.erase(0, begin);
	mContent += temp;
	while (std::getline(ifs, temp))
		mContent += temp;
	PH_LOG(LogType::Info, std::string("Xml loadFromFile(): ") + mContent);
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

	begin = mContent.find(name, begin + 1);
	if (begin == std::string::npos)
		PH_EXCEPTION("cannot find child");
	++begin; // WARNING: Don't use += name.size() here if you want to keep tag name

	std::size_t end = mContent.find('>', begin);
	if (end == std::string::npos)
		PH_EXCEPTION("missing angle bracket in child opening tag");

	if (isSelfClosingTag(end))
		++end;
	else {
		name.insert(1, "/");
		name.push_back('>');

		end = mContent.find(name, end + 1);
		if (end == std::string::npos)
			PH_EXCEPTION("missing closing tag in child");
	}

	Xml xml;
	xml.mContent = mContent.substr(begin, end - begin);
	PH_LOG(LogType::Info, "Xml getChild(): " + xml.mContent);
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

	begin = mContent.find(name, begin + 1);
	if (begin == std::string::npos)
		return std::vector<Xml>();
	++begin; // WARNING: Don't use += name.size() here if you want to keep tag name

	std::size_t end = mContent.find('>', begin);
	if (end == std::string::npos)
		PH_EXCEPTION("missing angle bracket in child opening tag");

	std::string endingTag = name;
	endingTag.insert(1, "/");
	endingTag.push_back('>');

	std::vector<Xml> children;
	if (isSelfClosingTag(end)) {
		++end;
		Xml xml;
		xml.mContent = mContent.substr(begin, end - begin);
		children.push_back(xml);
		begin = mContent.find(name, end);
	}
	else {
		end = mContent.find(endingTag, end + 1);
		if (end == std::string::npos)
			PH_EXCEPTION("missing closing tag in child");

		Xml xml;
		xml.mContent = mContent.substr(begin, end - begin);
		PH_LOG(LogType::Info, "Xml getChildren(): " + xml.mContent);
		children.push_back(xml);
		begin = mContent.find(name, end + endingTag.size());
	}
	while (begin != std::string::npos) {
		++begin; // WARNING: Don't use += name.size() here if you want to keep tag name

		end = mContent.find('>', begin);
		if (end == std::string::npos)
			PH_EXCEPTION("missing angle bracket in child opening tag");
		if (isSelfClosingTag(end)) {
			++end;
			Xml xml;
			xml.mContent = mContent.substr(begin, end - begin);
			children.push_back(xml);
			begin = mContent.find(name, end);
		}
		else {
			end = mContent.find(endingTag, end + 1);
			if (end == std::string::npos)
				PH_EXCEPTION("missing closing tag in child");

			Xml xml;
			xml.mContent = mContent.substr(begin, end - begin);
			PH_LOG(LogType::Info, "Xml getChildren(): " + xml.mContent);
			children.push_back(xml);
			begin = mContent.find(name, end + endingTag.size());
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
	std::size_t begin = mContent.find(name);
	if (begin == std::string::npos)
		PH_EXCEPTION("attribute name cannot be found");
	begin += name.size();

	std::size_t end = mContent.find('>');
	if (end == std::string::npos)
		PH_EXCEPTION("missing angle bracket in child opening tag");
	if (isSelfClosingTag(end))
		--end;
	if (begin >= end)
		PH_EXCEPTION("attribute name cannot be found");
	if (isEmptyAttributeValue(begin))
		return Xml();

	end = mContent.find('\"', begin + 1);
	if (end == std::string::npos)
		PH_EXCEPTION("missing closing quote");

	Xml xml;
	xml.mContent = mContent.substr(begin, end - begin);
	PH_LOG(LogType::Info, "Xml getAttribute(): " + xml.mContent);
	return xml;
}

std::string Xml::toString() const
{
	std::size_t begin = findEndOfCurrentTagAttributes();
	if (begin == std::string::npos)
		return mContent;
	++begin;
	return mContent.substr(begin, mContent.size() - begin);
}

}