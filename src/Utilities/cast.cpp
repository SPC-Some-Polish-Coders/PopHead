#include "cast.hpp"
#include "Logs/logs.hpp"

#include <unordered_map>

namespace ph {

unsigned Cast::toUnsigned(const std::string& str)
{
	const unsigned long ulResult = std::stoul(str);
	const unsigned result = ulResult;
	if (result != ulResult)
		PH_EXCEPTION("unsigned type is too small to contain conversion result");
	return result;
}

std::string Cast::toString(const sf::Vector2f& vec)
{
	std::string xVal = std::to_string(vec.x);
	std::string yVal = std::to_string(vec.y);
	return "x:" + xVal + " y:" + yVal;
}

bool Cast::toBool(const std::string& str)
{
	if (str == "true" || str == "1")
		return true;
	else if (str == "false" || str == "0")
		return false;
	else
		PH_EXCEPTION("Cast to bool failed!");
}

ObjectType Cast::toObjectType(const std::string& str)
{
	for (auto& it : objectTypeMap)
	{
		if (str.find(it.first) != std::string::npos)
			return it.second;
	}
		PH_EXCEPTION("There is no such ObjectType!");
}

}