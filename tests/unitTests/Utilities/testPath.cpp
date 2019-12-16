#include <catch.hpp>

#include "Utilities/filePath.hpp"
#include "Logs/criticalError.hpp"
#include "../TestsUtilities/bufferedHandler.hpp"

#include <string>

namespace ph {

	TEST_CASE("Filepath is properly converted to file name", "[Utilities][FilePath]")
	{
		std::string terminalFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Terminal" + PH_PATH_SEPARATOR + "terminal.cpp";
		std::string rendererFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Renderer" + PH_PATH_SEPARATOR + "renderer.cpp";
		std::string musicPlayerFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Audio" + PH_PATH_SEPARATOR + "Music" + 
			PH_PATH_SEPARATOR + "musicPlayer.cpp";

		CHECK(FilePath::toFilename(terminalFilePath) == "terminal.cpp");
		CHECK(FilePath::toFilename(rendererFilePath) == "renderer.cpp");
		CHECK(FilePath::toFilename(musicPlayerFilePath) == "musicPlayer.cpp");
	}
}
