#ifndef POPHEAD_UTILITIES_INILOADER_H_
#define	POPHEAD_UTILITIES_INILOADER_H_

#include "Logs/log.hpp"
#include <fstream>
#include <vector>

namespace PopHead {
namespace Utilies {

class iniLoader
{
private:
	static std::fstream iniSettingsFile;
protected:
	static void OpenTheFile();
	static void CloseTheFile();

//Probably there will be many derived classes which correspond to
//the proper module - each of them will be public with public members.

};

}}

#endif //!POPHEAD_UTILITIES_INILOADER_H_

