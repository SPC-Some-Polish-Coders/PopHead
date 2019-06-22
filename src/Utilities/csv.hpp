#pragma once

#include <string>
#include <vector>

namespace ph {

namespace Csv {
	std::vector<std::string> toStrings(const std::string& csv);

	std::vector<int> toInts(const std::string& csv);

	std::vector<unsigned> toUnsigneds(const std::string& csv);
}

}
