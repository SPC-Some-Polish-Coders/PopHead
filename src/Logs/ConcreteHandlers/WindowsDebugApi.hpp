#pragma once

#include "Logs/handler.hpp"

namespace ph {

	class WindowsDebugApiHandler : public Handler
	{
	public:

	private:
		virtual void utilizeLog(const LogRecord& logRecord);
	};
}
