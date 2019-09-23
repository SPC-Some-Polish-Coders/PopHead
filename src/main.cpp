#include "game.hpp"

#include "Logs/logs.hpp"
#include "Gui/messageBox.hpp"

#include <stdexcept>
#include <string>

#include "Logs/logger.hpp"
#include "Logs/logsInitializing.hpp"

#include "Gui/xmlGuiParser.hpp"
#include "Gui/guiActionsParserImpl.hpp"

int main()
{
	try {
		PH_LOG_INFO("start initializing PopHead");
		ph::Game game;

		ph::XmlGuiParser::setActionsParser(std::make_unique<ph::GuiActionsParserImpl>());

		// TODO: change place of initializing logs to start of main(), because now it needs Terminal from Game
		ph::initializeLogsModule("config/logsConfig.ini", game.getTerminal());
		
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
