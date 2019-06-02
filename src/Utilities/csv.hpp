#ifndef POPHEAD_UTILITIES_CSV_H_
#define POPHEAD_UTILITIES_CSV_H_

#include <string>
#include <vector>

namespace PopHead {
namespace Utilities {

namespace Csv {
	std::vector<int> toIntValues(const std::string& csv);
}

}}

#endif // !POPHEAD_UTILITIES_CSV_H_
