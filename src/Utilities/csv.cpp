#include "pch.hpp"
#include "csv.hpp"
#include "Utilities/cast.hpp"

namespace ph {

std::vector<std::string> Csv::toStrings(const std::string& csv)
{
	std::istringstream iss(csv);
	std::vector<std::string> values;
	std::string temp;
	while (std::getline(iss, temp, ',')) 
		values.push_back(temp);
	return values;
}

std::vector<i32> Csv::toI32s(const std::string& csv)
{
	std::istringstream iss(csv);
	std::vector<i32> values;
	std::string temp;
	while (std::getline(iss, temp, ',')) {
		const i32 value = std::stoi(temp);
		values.push_back(value);
	}
	return values;
}

std::vector<u32> Csv::toU32s(const std::string& csv)
{
	std::istringstream iss(csv);
	std::vector<u32> values;
	std::string temp;
	while (std::getline(iss, temp, ',')) {
		const u32 value = toU32(temp);
		values.push_back(value);
	}
	return values;
}

}
