#ifndef POPHEAD_LOGS_LOGGER_H_
#define POPHEAD_LOGS_LOGGER_H_

#include "log.hpp"
#include "logSettings.hpp"
#include "Utilities/path.hpp"
#include <SFML/System.hpp>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>

namespace ph {

class GameData;

class Logger
{
private:
	Logger();

public:
	Logger(Logger&) = delete;
	void operator=(Logger&) = delete;

	static Logger& getInstance(){
		static Logger Logger; 
		return Logger;
	}

	void setGameData(GameData* gameData) { mGameData = gameData; }

	auto getLogSettings() -> LogSettings& { return mLogSettings; }

	void writeLog(const LogData& log);

private:
	void openFile();
	void saveLogInFile(const LogData& log); 
	void writeLogInConsole(const LogData& log);
	void writeLogInInternalTerminal(const LogData& log);
	std::stringstream printLog(const LogData& log);
	std::string nameTheFile();
	sf::Time getElapsedTimeSinceCreation();

private:
	LogSettings mLogSettings;
	std::ofstream mLogFile;
	sf::Clock mClock;
	GameData* mGameData;
};

}

#endif // !POPHEAD_LOGS_LOGGER_H_