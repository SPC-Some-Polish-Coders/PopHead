#pragma once

#include "Logs/handler.hpp"
#include <vector>

namespace Tests {

	class BufferedHandler : public ph::Handler
	{
	public:
		ph::LogRecord getLogRecordFromStart(size_t index = 0) const;
		ph::LogRecord getLogRecordFromEnd(size_t index = 0) const;

		size_t getRecordsCount() const;

		void clearRecords();

	private:
		virtual void utilizeLog(const ph::LogRecord& logRecord);

		static std::vector<ph::LogRecord> mLogRecords;
	};
}
