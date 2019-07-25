#include "cast.hpp"
#include "Logs/logs.hpp"

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
	if(str.find("zombie") != std::string::npos)
		return ObjectType::Zombie;
	else if (str.find("npc") != std::string::npos)
		return ObjectType::Npc;
	else
		PH_EXCEPTION("There is not such ObjectType!");
}

}