#include "testHandler.hpp"

namespace Tests {
	
	std::vector<ph::LogRecord> TestHandler::mLogRecords = {};

	ph::LogRecord TestHandler::getLogRecordFromStart(size_t index) const
	{
		return mLogRecords.at(index);
	}

	ph::LogRecord TestHandler::getLogRecordFromEnd(size_t index) const
	{
		return mLogRecords.at(mLogRecords.size() - 1 - index);
	}

	size_t TestHandler::getRecordsCount() const
	{
		return mLogRecords.size();
	}

	void TestHandler::clearRecords()
	{
		mLogRecords.clear();
	}

	void TestHandler::utilizeLog(const ph::LogRecord& logRecord)
	{
		mLogRecords.emplace_back(logRecord);
	}
}
