#include "game.hpp"

#include "Utilities/debug.hpp"
#include "Gui/messageBox.hpp"

#include <stdexcept>
#include <string>

#include "Logs/logger.hpp"
#include "Logs/logsInitializing.hpp"

int main()
{
	try {
		PH_LOG(ph::LogType::Info, "start executing PopHead!");
		ph::Game game;
		ph::initializeLogsModule("../logsConfig.ini", game.getTerminal());
		game.run();
	}
	catch (const std::exception& e) {
		PH_LOG(ph::LogType::UnhandledException, e.what());
		ph::showErrorMessageBox("Error", e.what());
		throw;
	}
	catch (...) {
		PH_LOG(ph::LogType::UnhandledException, "Unknown error occurred!");
		ph::showErrorMessageBox("Error", "Unknown error occurred!");
		throw;
	}
	return 0;
}
