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

/*
	NOTE: There is always only one "root" tag in every getChild() and getChildren() calls which has children.
	TODO: Disallow to have more than one root tag? It could be done in loadFromFile() probably. But is it necessary? VS informs when it happens. But what about external users?

	Example implementation of getChild():
	Find first <.
	Check is it equel to name by finding "space" character or closing angle bracket.
	* If yes return.
	* If not check if is it self closing tag
		* If yes seach for next <
		* If no seach for closing tag (but be carefull about tags with same name. Do it until count is not equel 0. What if some tag won't have closing tag?
*/
Xml Xml::getChild(std::string name) const
{
	PH_ASSERT(!name.empty(), "child name cannot be empty");
	std::size_t begin = findEndOfCurrentTagAttributes();
	PH_ASSERT(begin != std::string::npos, "missing closing angle bracket");
	if (isSelfClosingTag(begin))
		PH_EXCEPTION("current tag cannot have children");
	while (true) {
		begin = mContent.find('<', begin + 1);
		if (begin == std::string::npos)
			PH_EXCEPTION("cannot find child");
		++begin;
		/*
			NOTE: 
			* If it find space -> tag with attributes
			* If it find closing angle breacket -> tag without attributes
			WARNING: It will fail with tags like <foo   />
			TODO:
				* Delete unnecessary spaces in loadFromFile() ?
				* Or maybe just use one more find ?

			Add characters like \t to find_first_of()?
			Create method like isTagWithAttributes() and put find_first_of() there?
		*/
		std::size_t end = mContent.find_first_of(" >", begin + 1);
		if (end == std::string::npos)
			PH_EXCEPTION("missing angle bracket in child opening tag");
		if (isSelfClosingTag(end)) {
			if (mContent.compare(begin, end - begin - 1, name) == 0) {
				Xml xml;
				xml.mContent = mContent.substr(begin, end - begin + 1);
				PH_LOG(LogType::Info, "Xml getChild(): " + xml.mContent);
				return xml;
			}
			else
				begin = end;
		}
		else {
			if (mContent.compare(begin, end - begin, name) == 0) {
				unsigned count = 0;
				// TODO: If it is tag with attributes skip them ???
				while (true) {
					end = mContent.find(name, end + 2);
					if (end == std::string::npos)
						PH_EXCEPTION("missing child closing tag");
					// NOTE: Is closing tag
					if (mContent[end - 2] == '<' && mContent[end - 1] == '/' && count == 0) {
						Xml xml;
						xml.mContent = mContent.substr(begin, end - begin - 2);
						PH_LOG(LogType::Info, "Xml getChild(): " + xml.mContent);
						return xml;
					}
					else if (mContent[end - 2] == '<' && mContent[end - 1] == '/') {
						--count;
						end += name.size();
					}
					else if (mContent[end - 1] == '<') {
						end = findEndOfCurrentTagAttributes(end + name.size());
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
				// NOTE: Set begin to > of ending tag. Be carefull about childs with same name as parent.
				// TODO: If it is tag with attributes skip them ???
				const std::string currentTagName = mContent.substr(begin, end - begin);
				begin = end;
				unsigned count = 0;
				while (true) {
					begin = mContent.find(currentTagName, begin + 2);
					if (begin == std::string::npos)
						PH_EXCEPTION("missing child closing tag");
					// NOTE: Is closing tag
					if (mContent[begin - 2] == '<' && mContent[begin - 1] == '/' && count == 0) {
						begin += currentTagName.size();
						break;
					}
					else if (mContent[begin - 2] == '<' && mContent[begin - 1] == '/') {
						--count;
						begin += currentTagName.size();
					}
					else if (mContent[begin - 1] == '<') {
						begin = findEndOfCurrentTagAttributes(begin + currentTagName.size());
						if (!isSelfClosingTag(begin))
							++count;
					}
					else
						begin += currentTagName.size() - 1;
				}
			}
		}
	}
}

std::vector<Xml> Xml::getChildren(std::string name) const
{
	PH_ASSERT(!name.empty(), "child name cannot be empty");
	name.insert(0, "<");

	std::size_t begin = findEndOfCurrentTagAttributes();
	PH_ASSERT(begin != std::string::npos, "missing closing angle bracket");
	if (isSelfClosingTag(begin))
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

bool Xml::hasAttribute(std::string name) const
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

	std::size_t begin = mContent.find(name);
	if (begin == std::string::npos) {
		PH_LOG(LogType::Info, "Xml hasAttribute(): false");
		return false;
	}
	begin += name.size();

	std::size_t end = mContent.find('>');
	if (end == std::string::npos)
		PH_EXCEPTION("missing angle bracket in child opening tag");
	if (isSelfClosingTag(end))
		--end;
	if (begin >= end) {
		PH_LOG(LogType::Info, "Xml hasAttribute(): false");
		return false;
	}
	if (isEmptyAttributeValue(begin)) {
		PH_LOG(LogType::Info, "Xml hasAttribute(): true");
		return true;
	}

	end = mContent.find('\"', begin + 1);
	if (end == std::string::npos)
		PH_EXCEPTION("missing closing quote");

	PH_LOG(LogType::Info, "Xml hasAttribute(): true");
	return true;
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