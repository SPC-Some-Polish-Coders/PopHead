#include "pch.hpp"
#include "messageBox.hpp"

#ifdef PH_WINDOWS
#include <Windows.h>
#endif // PH_WINDOWS

namespace ph {

void showErrorMessageBox(const char* title, const std::string& message)
{
#ifdef PH_WINDOWS
	MessageBoxA(nullptr, message.c_str(), title, MB_OK | MB_ICONERROR);
#endif // PH_WINDOWS
}

}
