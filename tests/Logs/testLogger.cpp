#include <catch.hpp>

#include "Logs/logger.hpp"
#include "../TestsUtilities/testHandler.hpp"

#include <vector>

namespace ph {

	TEST_CASE("Logger creates correct log record", "[Logs][Logger]")
	{
		Tests::TestHandler handler;
		handler.clearRecords();

		const static std::vector<ph::LogLevel> logLevels{ LogLevel::Info, LogLevel::Warning, LogLevel::Error, LogLevel::Critical };

		for (std::size_t i = 0; i < logLevels.size(); ++i)
		{
			DYNAMIC_SECTION("Checking LogLevel::" << logLevelToString(logLevels.at(i)))
			{
				Logger::createLog(logLevels.at(i), "my test message", __FILE__, static_cast<unsigned short>(i * 20));

				CHECK(handler.getRecordsCount() == 1);
				auto logRecord = handler.getLogRecordFromEnd();

				CHECK(logRecord.level == logLevels.at(i));
				CHECK(logRecord.fileLine == i * 20);
				CHECK(logRecord.fileName == "testLogger.cpp");
				CHECK(logRecord.message == "my test message");
				CHECK(logRecord.moduleName == "Tests");
			}
		}
	}
}
