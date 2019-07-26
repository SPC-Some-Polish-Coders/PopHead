#include <catch.hpp>

#include "Utilities/path.hpp"
#include "Logs/criticalError.hpp"
#include "../TestsUtilities/testHandler.hpp"

#include <string>

namespace ph {

	TEST_CASE("Filepath is properly converted to module name", "[Utilities][Path]")
	{
		std::string audioModuleFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Audio" + PH_PATH_SEPARATOR;
		std::string inputModuleFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Input" + PH_PATH_SEPARATOR;
		std::string utilitiesModuleFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Utilities" + PH_PATH_SEPARATOR;
		std::string mainFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "main.cpp";

		CHECK(Path::toModuleName(audioModuleFilePath) == "Audio");
		CHECK(Path::toModuleName(inputModuleFilePath) == "Input");
		CHECK(Path::toModuleName(utilitiesModuleFilePath) == "Utilities");
		CHECK(Path::toModuleName(mainFilePath) == "Main directory");
	}

	TEST_CASE("Filepath in tests/ is properly converted to module name", "[Utilities][Path]")
	{
		std::string audioTestFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "tests" + PH_PATH_SEPARATOR + "Audio" + PH_PATH_SEPARATOR;
		std::string inputTestFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "tests" + PH_PATH_SEPARATOR + "Input" + PH_PATH_SEPARATOR;
		std::string utilitiesTestFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "tests" + PH_PATH_SEPARATOR + "Utilities" + PH_PATH_SEPARATOR;
		std::string mainTestFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "tests" + PH_PATH_SEPARATOR + "main.cpp";

		CHECK(Path::toModuleName(audioTestFilePath) == "Tests");
		CHECK(Path::toModuleName(inputTestFilePath) == "Tests");
		CHECK(Path::toModuleName(utilitiesTestFilePath) == "Tests");
		CHECK(Path::toModuleName(mainTestFilePath) == "Tests");
	}

	TEST_CASE("Filepath is properly converted to file name", "[Utilities][Path]")
	{
		std::string terminalFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Terminal" + PH_PATH_SEPARATOR + "terminal.cpp";
		std::string rendererFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Renderer" + PH_PATH_SEPARATOR + "renderer.cpp";
		std::string musicPlayerFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR + "src" + PH_PATH_SEPARATOR + "Audio" + PH_PATH_SEPARATOR + "Music" + 
			PH_PATH_SEPARATOR + "musicPlayer.cpp";

		CHECK(Path::toFilename(terminalFilePath) == "terminal.cpp");
		CHECK(Path::toFilename(rendererFilePath) == "renderer.cpp");
		CHECK(Path::toFilename(musicPlayerFilePath) == "musicPlayer.cpp");
	}

	TEST_CASE("Bad paths are handled properly", "[Utilities][Path]")
	{
		std::string nonsens = "sdifjnsdjfkgnjfsdhgjsdfgbr";
		std::string nonexistentFilePath = PH_PATH_SEPARATOR+ std::string("PopHead") + PH_PATH_SEPARATOR + "github" + PH_PATH_SEPARATOR + "git" + PH_PATH_SEPARATOR;
		std::string incompleteFilePath = PH_PATH_SEPARATOR + std::string("PopHead") + PH_PATH_SEPARATOR +"src" + PH_PATH_SEPARATOR;

		Tests::TestHandler logs;
		logs.clearRecords();

		CHECK_THROWS_AS(Path::toModuleName(nonsens), ph::CriticalError);
		CHECK_THROWS_AS(Path::toModuleName(nonexistentFilePath), ph::CriticalError);
		CHECK_THROWS_AS(Path::toModuleName(incompleteFilePath), ph::CriticalError);

		CHECK(logs.getRecordsCount() == 3);
	}
}
