#ifndef PH_TESTS

#include "game.hpp"

#include "Logs/logs.hpp"
#include "Gui/messageBox.hpp"

#include <stdexcept>
#include <string>

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main()
{
	try {
		PH_LOG(ph::LogLevel::Info, "start executing PopHead!");
		ph::Game game;
		game.run();
	}
	catch (const std::exception& e) {
		//PH_LOG(ph::LogLevel::UnhandledException, e.what());
		ph::showErrorMessageBox("Error", e.what());
		throw;
	}
	catch (...) {
		//PH_LOG(ph::LogLevel::UnhandledException, "Unknown error occurred!");
		ph::showErrorMessageBox("Error", "Unknown error occurred!");
		throw;
	}

	return 0;
}

#else // !PH_TESTS

#include "TestsUtilities/bufferedHandler.hpp"
#include "Logs/logger.hpp"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main()
{
	std::unique_ptr<ph::Handler> bufferedHandler(new Tests::BufferedHandler);
	bufferedHandler->enableAllModules();
	bufferedHandler->enableAllLogLevels();
	ph::Logger::addLogsHandler(std::move(bufferedHandler));
	return Catch::Session().run();
}

#endif // !PH_TESTS
