#ifndef POPHEAD_UTILITIES_INILOADER_H_
#define	POPHEAD_UTILITIES_INILOADER_H_

#include <fstream>
#include <vector>

namespace PopHead {
namespace Utilities {

class IniLoader
{
public:
	//Some pretty cool funcs will be here
protected:
	static void openTheFile();
	static void closeTheFile();
	static bool findPhrase(std::string);
	static bool findValue(std::string);
private:
	static std::fstream iniSettingsFile;
	static std::string currentLine;
};

}}

#endif //!POPHEAD_UTILITIES_INILOADER_H_

