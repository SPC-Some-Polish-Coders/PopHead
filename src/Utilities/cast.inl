#include "cast.hpp"
#include "Logs/logs.hpp"
#include "aliases.hpp"
#include <unordered_map>

namespace ph {

u32 castToU32(const std::string& str)
{
	u64 ulResult = std::stoul(str);
	u32 result = Cast<u32>(ulResult);
	PH_ASSERT_CRITICAL(result == ulResult, "u32 type is too small to contain conversion result");
	return result;
}

bool castToBool(const std::string& str)
{
	if (str == "true" || str == "1")
		return true;
	else if (str == "false" || str == "0")
		return false;
	else
		PH_EXIT_GAME("Cast to bool failed!");
}

std::string castToString(Vec2 vec)
{
	std::string xVal = std::to_string(vec.x);
	std::string yVal = std::to_string(vec.y);
	return "x:" + xVal + " y:" + yVal;
}

Vec4 castToNormalizedColorVec4(sf::Color c)
{
	return Vec4({
		Cast<float>(c.r) / 255.f, Cast<float>(c.g) / 255.f,
		Cast<float>(c.b) / 255.f, Cast<float>(c.a) / 255.f
	});
}

sf::Color castToPackedColor(Vec4 c)
{
	return sf::Color(
		Cast<u8>(c.r * 255.f), Cast<u8>(c.g * 255.f),
		Cast<u8>(c.b * 255.f), Cast<u8>(c.a * 255.f)
	);
}

}
