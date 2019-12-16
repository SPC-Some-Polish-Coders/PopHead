#include "game.hpp"
#include "Logs/logs.hpp"
#include "Logs/logger.hpp"
#include "Logs/logsInitializing.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "GUI/guiActionsParserImpl.hpp"
#include "Utilities/profiling.hpp"
#include "GUI/messageBox.hpp"
#include <stdexcept>
#include <string>

int main()
{
	try {
		PH_BEGIN_PROFILING_SESSION("PopHead initializing", "initProfilingResults.json");

		PH_LOG_INFO("start initializing PopHead");
		ph::Game game;

		ph::XmlGuiParser::setActionsParser(std::make_unique<ph::GuiActionsParserImpl>());

		// TODO: change place of initializing logs to start of main(), because now it needs Terminal from Game
		ph::initializeLogsModule("config/logsConfig.ini", game.getTerminal());

		PH_END_PROFILING_SESSION();
		
		PH_BEGIN_PROFILING_SESSION("PopHead runtime", "runtimeProfilingResults.json");

		PH_LOG_INFO("start executing PopHead");
		game.run();

		PH_END_PROFILING_SESSION();
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
