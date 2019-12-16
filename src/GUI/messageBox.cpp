#include "messageBox.hpp"

#ifdef PH_WINDOWS
#include <Windows.h>
#endif // PH_WINDOWS

namespace ph {

void showErrorMessageBox(const std::string& title, const std::string& message)
{
#ifdef PH_WINDOWS
	MessageBoxA(nullptr, message.c_str(), title.c_str(), MB_OK | MB_ICONERROR);
#endif // PH_WINDOWS
}

}