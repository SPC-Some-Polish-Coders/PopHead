#include <catch.hpp>

#include "Utilities/filePath.hpp"
#include "Logs/criticalError.hpp"
#include "../TestsUtilities/bufferedHandler.hpp"

#include <string>

namespace ph {

	TEST_CASE("Filepath is properly converted to module name", "[Utilities][FilePath]")
	{
		std::string audioModuleFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Audio" + PH_PATH_SEPARATOR;
		std::string inputModuleFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Input" + PH_PATH_SEPARATOR;
		std::string utilitiesModuleFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Utilities" + PH_PATH_SEPARATOR;
		std::string mainFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "main.cpp";

		CHECK(FilePath::toModuleName(audioModuleFilePath) == "Audio");
		CHECK(FilePath::toModuleName(inputModuleFilePath) == "Input");
		CHECK(FilePath::toModuleName(utilitiesModuleFilePath) == "Utilities");
		CHECK(FilePath::toModuleName(mainFilePath) == "MainDirectory");
	}

	TEST_CASE("Filepath in tests/ is properly converted to module name", "[Utilities][FilePath]")
	{
		std::string audioTestFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "tests" + PH_PATH_SEPARATOR + "Audio" + PH_PATH_SEPARATOR;
		std::string inputTestFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "tests" + PH_PATH_SEPARATOR + "Input" + PH_PATH_SEPARATOR;
		std::string utilitiesTestFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "tests" + PH_PATH_SEPARATOR + "Utilities" + PH_PATH_SEPARATOR;
		std::string mainTestFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "tests" + PH_PATH_SEPARATOR + "main.cpp";

		CHECK(FilePath::toModuleName(audioTestFilePath) == "Tests");
		CHECK(FilePath::toModuleName(inputTestFilePath) == "Tests");
		CHECK(FilePath::toModuleName(utilitiesTestFilePath) == "Tests");
		CHECK(FilePath::toModuleName(mainTestFilePath) == "Tests");
	}

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

	TEST_CASE("Bad paths are handled properly", "[Utilities][FilePath]")
	{
		std::string nonsens = "sdifjnsdjfkgnjfsdhgjsdfgbr";
		std::string nonexistentFilePath = PH_PATH_SEPARATOR+ std::string("PopHead") + PH_PATH_SEPARATOR + "github" + PH_PATH_SEPARATOR + "git" + PH_PATH_SEPARATOR;
		std::string incompleteFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR +"src" + PH_PATH_SEPARATOR;

		Tests::BufferedHandler logs;
		logs.clearRecords();

		CHECK_THROWS_AS(FilePath::toModuleName(nonsens), ph::CriticalError);
		CHECK_THROWS_AS(FilePath::toModuleName(nonexistentFilePath), ph::CriticalError);
		CHECK_THROWS_AS(FilePath::toModuleName(incompleteFilePath), ph::CriticalError);

		CHECK(logs.getRecordsCount() == 3);
	}
}
