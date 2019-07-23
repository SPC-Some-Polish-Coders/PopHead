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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#endif // !PH_TESTS
