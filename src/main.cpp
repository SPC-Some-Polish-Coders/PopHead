#include "pch.hpp"
#include "game.hpp"
#include "Logs/logger.hpp"
#include "Logs/logsInitializing.hpp"
#include "Utilities/messageBox.hpp"
#include "Resources/soundBufferHolder.hpp"
#include "Audio/Music/musicPlayer.hpp"

int main()
{
	try {
		PH_BEGIN_PROFILING_SESSION("initProfilingResults.json");

		PH_LOG_INFO("start initializing PopHead");
		ph::Game game;

		// TODO: change place of initializing logs to start of main(), because now it needs Terminal from Game
		ph::initializeLogsModule("config/logsConfig.ini");

		PH_END_PROFILING_SESSION();
		
		PH_BEGIN_PROFILING_SESSION("runtimeProfilingResults.json");

		PH_LOG_INFO("start executing PopHead");
		game.run();

		PH_END_PROFILING_SESSION();

		ph::clearSoundBufferHolder();
		ph::MusicPlayer::clearMusicPlayer();
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
