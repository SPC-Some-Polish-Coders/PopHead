#ifndef POPHEAD_UTILITIES_INILOADER_H_
#define	POPHEAD_UTILITIES_INILOADER_H_

#include <fstream>
#include <vector>

namespace PopHead {
namespace Utilities {

class IniLoader
{
public:
	//static int iniGetWindowWidth();
	//static int iniGetWindowHeight();
	//static bool iniGetFullscreenMode(); 
protected:
	static void openTheFile();
	static void closeTheFile();
	static bool findPhrase(const std::string&);
	static bool findValue(const std::string&);	
	static bool getBool(const std::string&);
protected:
	static std::string currentLine;
private:
	static std::fstream iniSettingsFile;
};

}}

#endif //!POPHEAD_UTILITIES_INILOADER_H_

