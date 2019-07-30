#pragma once

#include "Logs/handler.hpp"

namespace ph {

	class ConsoleHandler : public Handler
	{
	public:

	private:
		virtual void utilizeLog(const LogRecord& logRecord);
	};
}
