#ifndef POPHEAD_UTILITIES_INILOADER_H_
#define	POPHEAD_UTILITIES_INILOADER_H_

#include "Logs/log.hpp"
#include <fstream>
#include <vector>

namespace PopHead {
namespace Utilities {

class IniLoader
{
public:
	static bool iniGetShouldLogIntoConsole();
	static bool iniGetShouldLogIntoFile();
	static std::vector<Logs::LogType> iniGetLogTypesToWrite();
	static std::vector<std::string> iniGetModuleNamesToWrite();
private:
	static std::fstream iniSettingsFile;
	static std::string currentLine;
protected:
	static void openTheFile();
	static void closeTheFile();
	static bool findPhrase(std::string);
	static bool findValue(std::string);
};

}}

#endif //!POPHEAD_UTILITIES_INILOADER_H_

