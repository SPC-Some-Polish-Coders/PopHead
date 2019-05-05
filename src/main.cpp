#include "Base/game.hpp"

#include "Logs/Logs.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32
#include <stdexcept>
#include <cstdlib>

int main()
{
	try {
		PopHead::LOG(LogType::INFO, ModuleID::None, "start executing PopHead!");
		PopHead::Base::Game();
	}
	catch (const std::exception& e) {
#ifdef _WIN32
		MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
#endif // _WIN32
		return EXIT_FAILURE;
	}
	catch (...) {
#ifdef _WIN32
		MessageBoxA(nullptr, "Unknown error occurred!", "Error", MB_OK | MB_ICONERROR);
#endif // _WIN32
		return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}
