#ifdef PH_TESTS

#include "TestsUtilities/bufferedHandler.hpp"
#include "Logs/logger.hpp"
#include <memory>

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main()
{
	std::unique_ptr<ph::Handler> bufferedHandler(new Tests::BufferedHandler);
	bufferedHandler->enableAllPaths();
	bufferedHandler->enableAllLogLevels();
	ph::Logger::addLogsHandler(std::move(bufferedHandler));
	return Catch::Session().run();
}

#else

#include "game.hpp"

#include "GUI/messageBox.hpp"
#include "Logs/logs.hpp"
#include "Logs/logsInitializing.hpp"

#include <stdexcept>
#include <string>

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

int main()
{
	try {
		PH_LOG_INFO("start initializing PopHead");
		ph::Game game;

		// TODO: change place of initializing logs to start of main(), because now it needs Terminal from Game
		ph::initializeLogsModule("../config/logsConfig.ini", game.getTerminal());

		PH_LOG_INFO("start executing PopHead");
		game.run();
	}
	catch (const ph::CriticalError& criticalError) {
		ph::showErrorMessageBox("Critical Error: ", criticalError.what());
		throw;
	}
	catch (const std::exception& e) {
		PH_LOG_WARNING("Standard exceptions should be handled in code.");
		ph::showErrorMessageBox("Error", e.what());
		throw;
	}
	catch (...) {
		ph::showErrorMessageBox("Error", "Unknown error occurred!");
		throw;
	}

	return 0;
}

#endif // PH_TESTS

