#include "csv.hpp"
#include "Utilities/cast.hpp"
#include <sstream>

std::vector<std::string> ph::Csv::toStrings(const std::string& csv)
{
	std::istringstream iss(csv);
	std::vector<std::string> values;
	std::string temp;
	while (std::getline(iss, temp, ',')) 
		values.push_back(temp);
	return values;
}

std::vector<int> ph::Csv::toInts(const std::string& csv)
{
	std::istringstream iss(csv);
	std::vector<int> values;
	std::string temp;
	while (std::getline(iss, temp, ',')) {
		const int value = std::stoi(temp);
		values.push_back(value);
	}
	return values;
}

std::vector<unsigned> ph::Csv::toUnsigneds(const std::string& csv)
{
	std::istringstream iss(csv);
	std::vector<unsigned> values;
	std::string temp;
	while (std::getline(iss, temp, ',')) {
		const unsigned value = Cast::toUnsigned(temp);
		values.push_back(value);
	}
	return values;
}
