#include "Base/game.hpp"

#include "Logs/logs.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <stdexcept>
#include <cstdlib>
#include <string>

void showErrorMessageBox(const std::string& message, const std::string& title)
{
#ifdef _WIN32
	MessageBoxA(nullptr, message.c_str(), title.c_str(), MB_OK | MB_ICONERROR);
#endif // _WIN32
}

int main()
{
	try {
		PopHead::LOG(LogType::INFO, ModuleID::None, "start executing PopHead!");
		PopHead::Base::Game();
	}
	catch (const std::exception& e) {
		showErrorMessageBox(e.what(), "Error");
		return EXIT_FAILURE;
	}
	catch (...) {
		showErrorMessageBox("Unknown error occurred!", "Error");
		return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}
