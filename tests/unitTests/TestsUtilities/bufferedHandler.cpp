#include "bufferedHandler.hpp"

namespace Tests {
	
	std::vector<ph::LogRecord> BufferedHandler::mLogRecords = {};

	ph::LogRecord BufferedHandler::getLogRecordFromStart(size_t index) const
	{
		return mLogRecords.at(index);
	}

	ph::LogRecord BufferedHandler::getLogRecordFromEnd(size_t index) const
	{
		return mLogRecords.at(mLogRecords.size() - 1 - index);
	}

	size_t BufferedHandler::getRecordsCount() const
	{
		return mLogRecords.size();
	}

	void BufferedHandler::clearRecords()
	{
		mLogRecords.clear();
	}

	void BufferedHandler::utilizeLog(const ph::LogRecord& logRecord)
	{
		mLogRecords.emplace_back(logRecord);
	}
}
