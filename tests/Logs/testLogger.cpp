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

	TEST_CASE("You can add and remove Handler from Logger", "[Logs][Logger][Handler]")
	{
		class MockHandler : public Handler
		{
			virtual void utilizeLog(const LogRecord& logRecord) override {}
		};

		auto makePointer = []() -> std::unique_ptr<Handler> {
			std::unique_ptr<Handler> handler(new MockHandler);
			return std::move(handler);
		};

		auto handler1 = makePointer();
		auto handler2 = makePointer();
		auto handler3 = makePointer();
		auto handler4 = makePointer();

		SECTION("Single Handler")
		{
			const auto& ref = *handler1;
			Logger::addLogsHandler(std::move(handler1));
			CHECK(Logger::removeLogsHandler(ref));
		}
		SECTION("Multiple Handlers")
		{
			const auto& ref1 = *handler1;
			const auto& ref2 = *handler2;
			const auto& ref3 = *handler3;
			const auto& ref4 = *handler4;

			Logger::addLogsHandler(std::move(handler1));
			Logger::addLogsHandler(std::move(handler2));
			Logger::addLogsHandler(std::move(handler3));
			Logger::addLogsHandler(std::move(handler4));

			// order shouldn't be a problem
			CHECK(Logger::removeLogsHandler(ref3));
			CHECK(Logger::removeLogsHandler(ref1));
			CHECK(Logger::removeLogsHandler(ref4));
			CHECK(Logger::removeLogsHandler(ref2));
		}
		SECTION("Non-existing Handler")
		{
			const auto& ref = *handler1;
			CHECK(Logger::removeLogsHandler(ref) == false);
		}
	}
}
