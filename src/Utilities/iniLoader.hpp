#pragma once

#include <fstream>
#include <vector>

namespace ph {

class IniLoader
{
protected:
	static void openTheFile();
	static void closeTheFile();
	static bool findPhrase(const std::string&);
	static bool findValue(const std::string&);	
	static bool getBool(const std::string&);
private:
	static void handleException(const std::string&);

protected:
	static std::string currentLine;
private:
	static std::fstream iniSettingsFile;
};

}
