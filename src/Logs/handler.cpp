#include "handler.hpp"

namespace ph {

	void Handler::handleLog(const LogRecord& logRecord)
	{
		if (isPassedByFilter(logRecord))
			utilizeLog(logRecord);
	}

	bool Handler::isPassedByFilter(const LogRecord& logRecord)
	{
		return true;
	}
}
