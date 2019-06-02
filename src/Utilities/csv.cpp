#include "csv.hpp"
#include <sstream>

/*
	TODO:
	Possible improvements
	- CSV formats could vary in the choice of separator character.
	- Ignore separator character inside double-quote.
	- Allow embedded double-quote characters: Foo, bar, "Something, ""realy"" cool"
*/

std::vector<int> PopHead::Utilities::Csv::toIntValues(const std::string& csv)
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
