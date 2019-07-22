#include "catch.hpp"

#include "Utilities/path.hpp"

namespace ph {

namespace {
	std::string audioModuleFilePath = "\\PopHead\\src\\Audio\\";
	std::string inputModuleFilePath = "\\PopHead\\src\\Input\\";
	std::string utilitiesModuleFilePath = "\\PopHead\\src\\Utilities\\";
	std::string noneFilePath = "\\PopHead\\src\\main.cpp";

	std::string terminalFilePath = "\\PopHead\\src\\Terminal\\terminal.cpp";
	std::string rendererFilePath = "\\PopHead\\src\\Renderer\\renderer.cpp";
	std::string musicPlayerFilePath = "\\PopHead\\src\\Audio\\Music\\musicPlayer.cpp";

	std::string nonexistentFilePath = "\\PopHead\\github\\git\\repo";
	std::string duplicatedFilePath = "\\PopHead\\src\\PopHead\\src";
	std::string incompleteFilePath = "\\PopHead\\src\\";
}

TEST_CASE("is filepath properly converted to module name", "[Utilities][Path]")
{
	CHECK(Path::toModuleName(audioModuleFilePath) == "Audio");
	CHECK(Path::toModuleName(inputModuleFilePath) == "Input");
	CHECK(Path::toModuleName(utilitiesModuleFilePath) == "Utilities");
	CHECK(Path::toModuleName(noneFilePath) == "None");
}

TEST_CASE("is filepath properly converted to file name", "[Utilities][Path]")
{
	CHECK(Path::toFilename(terminalFilePath) == "terminal.cpp");
	CHECK(Path::toFilename(rendererFilePath) == "renderer.cpp");
	CHECK(Path::toFilename(musicPlayerFilePath) == "musicPlayer.cpp");
}

TEST_CASE("are exceptions called properly", "[Utilities][Path]")
{
	SECTION("lack of module") {
		CHECK_THROWS(Path::toModuleName(nonexistentFilePath));
	}
	SECTION("duplicated path") {
		CHECK_THROWS(Path::toModuleName(duplicatedFilePath));
	}
	SECTION("incomplete path") {
		CHECK_THROWS(Path::toModuleName(incompleteFilePath));
	}

}
}
