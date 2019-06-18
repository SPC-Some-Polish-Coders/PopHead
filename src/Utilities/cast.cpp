#include "cast.hpp"
#include "Utilities/debug.hpp"

unsigned ph::Cast::toUnsigned(const std::string& str)
{
	const unsigned long ulResult = std::stoul(str);
	const unsigned result = ulResult;
	if (result != ulResult)
		PH_EXCEPTION("unsigned type is too small to contain conversion result");
	return result;
}

std::string ph::Cast::toString(const sf::Vector2f& vec)
{
	std::string xVal = std::to_string(vec.x);
	std::string yVal = std::to_string(vec.y);
	return "x:" + xVal + " y:" + yVal;
}