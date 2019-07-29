#pragma once

#include "logRecord.hpp"
#include "handler.hpp"

#include <SFML/System/Clock.hpp>

#include <vector>
#include <memory>

namespace ph {

	class Logger
	{
	private:
		Logger() = default;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

	public:
		static void createLog(LogLevel level, const std::string& message, const std::string& fileName, unsigned short fileLine);

		static void addLogsHandler(std::unique_ptr<Handler> handler);
		static bool removeLogsHandler(const Handler& handler);

	private:
		static Logger& getInstance();

	private:
		std::vector<std::unique_ptr<Handler>> mHandlers;
		sf::Clock mClock;
	};
}
