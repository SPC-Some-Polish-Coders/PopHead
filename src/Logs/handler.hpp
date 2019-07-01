#pragma once

#include "logRecord.hpp"

#include <string>

namespace ph {
	
	class Handler
	{
	public:
		void handleLog(const LogRecord& logRecord);

	private:
		virtual void utilizeLog(const LogRecord& logRecord) = 0;

		bool isPassedByFilter(const LogRecord& logRecord);
	};
}