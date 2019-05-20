#include "iniLoader.hpp"
#include <iostream>

using PopHead::Logs::LogData;
using PopHead::Utilies::iniLoader;

std::fstream iniLoader::iniSettingsFile;

void iniLoader::OpenTheFile()
{
	iniSettingsFile.open("<relative path here>");
	if (!iniSettingsFile.is_open()) std::cout << "\n.ini file could not be opened\n";
}

void iniLoader::CloseTheFile()
{
	iniSettingsFile.close();
	if (iniSettingsFile.is_open()) std::cout << "\n.ini file could not be closed\n";
}