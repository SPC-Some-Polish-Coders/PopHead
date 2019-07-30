#include <catch.hpp>

#include "Logs/handler.hpp"
#include "mockHandler.hpp"

#include <fstream>

namespace ph {

	TEST_CASE("Handler at start is turned off", "[Logs][Handler]")
	{
		MockHandler handler;

		for (int i = 0; i < static_cast<int>(LogLevel::Count); ++i)
		{
			auto level = static_cast<LogLevel>(i);
			CHECK(!handler.isLogLevelAllowed(level));
		}

		std::ifstream modulesNames("config/modules.txt");

		std::string module;
		while (modulesNames >> module)
			CHECK(!handler.isModuleAllowed(module));
	}

	TEST_CASE("Modules can be enabled and disabled", "[Logs][Handler]")
	{
		MockHandler handler;

		handler.setModuleAllowing("Audio", true);
		handler.setModuleAllowing("Physics", true);
		handler.setModuleAllowing("Terminal", true);

		CHECK(handler.isModuleAllowed("Audio"));
		CHECK(handler.isModuleAllowed("Terminal"));
		CHECK(handler.isModuleAllowed("Physics"));

		handler.setModuleAllowing("Terminal", false);
		handler.setModuleAllowing("Audio", false);

		CHECK(!handler.isModuleAllowed("Terminal"));
		CHECK(handler.isModuleAllowed("Physics"));
		CHECK(!handler.isModuleAllowed("Audio"));
	}

	TEST_CASE("Log levels can be enabled and disabled", "[Logs][Handler]")
	{
		MockHandler handler;

		handler.setLogLevelAllowing(LogLevel::Info, true);
		handler.setLogLevelAllowing(LogLevel::Critical, true);
		handler.setLogLevelAllowing(LogLevel::Warning, true);

		CHECK(handler.isLogLevelAllowed(LogLevel::Info));
		CHECK(handler.isLogLevelAllowed(LogLevel::Warning));
		CHECK(handler.isLogLevelAllowed(LogLevel::Critical));

		handler.setLogLevelAllowing(LogLevel::Critical, false);
		handler.setLogLevelAllowing(LogLevel::Info, false);

		CHECK(!handler.isLogLevelAllowed(LogLevel::Info));
		CHECK(!handler.isLogLevelAllowed(LogLevel::Critical));
		CHECK(handler.isLogLevelAllowed(LogLevel::Warning));
	}

	TEST_CASE("All modules and levels can be enabled at once", "[Logs][Handler]")
	{
		MockHandler handler;
		handler.enableAllModules();
		handler.enableAllLogLevels();

		for (int i = 0; i < static_cast<int>(LogLevel::Count); ++i)
		{
			auto level = static_cast<LogLevel>(i);
			CHECK(handler.isLogLevelAllowed(level));
		}

		std::ifstream modulesNames("config/modules.txt");

		std::string module;
		while (modulesNames >> module)
			CHECK(handler.isModuleAllowed(module));
	}
}
