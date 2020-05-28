#pragma once

#include <string>
#include <vector>

namespace ph {

namespace Csv {
	std::vector<std::string> toStrings(const std::string& csv);

	std::vector<i32> toI32s(const std::string& csv);

	std::vector<u32> toU32s(const std::string& csv);
}

}
