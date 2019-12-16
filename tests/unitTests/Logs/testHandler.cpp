#include <catch.hpp>

#include "Logs/handler.hpp"
#include "mockHandler.hpp"

#include <filesystem>

namespace ph {

	namespace {
		std::vector<std::string> getDirectoriesInSrc()
		{
			std::vector<std::string> directories;
			for (auto& p : std::filesystem::recursive_directory_iterator("src"))
				if (p.is_regular_file())
				{
					std::string module = p.path().generic_string();
					module.erase(0, 4);
					directories.emplace_back(std::move(module));
				}
			return directories;
		}
	}

	TEST_CASE("Handler at start is turned off", "[Logs][Handler]")
	{
		MockHandler handler;

		for (int i = 0; i < static_cast<int>(LogLevel::Count); ++i)
		{
			auto level = static_cast<LogLevel>(i);
			CHECK(!handler.isLogLevelAllowed(level));
		}

		auto directories = getDirectoriesInSrc();

		for (const auto& path : directories)
			CHECK(!handler.isPathAllowed(path));
	}

	TEST_CASE("Modules can be enabled and disabled", "[Logs][Handler]")
	{
		MockHandler handler;

		handler.setPathFilter("Audio", true);
		handler.setPathFilter("Physics", true);
		handler.setPathFilter("Terminal", true);

		CHECK(handler.isPathAllowed("Audio"));
		CHECK(handler.isPathAllowed("Terminal"));
		CHECK(handler.isPathAllowed("Physics"));

		handler.setPathFilter("Terminal", false);
		handler.setPathFilter("Audio", false);

		CHECK(!handler.isPathAllowed("Terminal"));
		CHECK(handler.isPathAllowed("Physics"));
		CHECK(!handler.isPathAllowed("Audio"));
	}

	TEST_CASE("Log levels can be enabled and disabled", "[Logs][Handler]")
	{
		MockHandler handler;

		handler.setLogLevelFilter(LogLevel::Info, true);
		handler.setLogLevelFilter(LogLevel::Critical, true);
		handler.setLogLevelFilter(LogLevel::Warning, true);

		CHECK(handler.isLogLevelAllowed(LogLevel::Info));
		CHECK(handler.isLogLevelAllowed(LogLevel::Warning));
		CHECK(handler.isLogLevelAllowed(LogLevel::Critical));

		handler.setLogLevelFilter(LogLevel::Critical, false);
		handler.setLogLevelFilter(LogLevel::Info, false);

		CHECK(!handler.isLogLevelAllowed(LogLevel::Info));
		CHECK(!handler.isLogLevelAllowed(LogLevel::Critical));
		CHECK(handler.isLogLevelAllowed(LogLevel::Warning));
	}

	TEST_CASE("All modules and levels can be enabled at once", "[Logs][Handler]")
	{
		MockHandler handler;
		handler.enableAllPaths();
		handler.enableAllLogLevels();

		for (int i = 0; i < static_cast<int>(LogLevel::Count); ++i)
		{
			auto level = static_cast<LogLevel>(i);
			CHECK(handler.isLogLevelAllowed(level));
		}

		auto directories = getDirectoriesInSrc();

		for (const auto& path : directories)
			CHECK(handler.isPathAllowed(path));
	}
}
