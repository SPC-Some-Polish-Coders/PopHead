#pragma once

#include "Logs/handler.hpp"

#include <fstream>

namespace ph {

	class FileHandler : public Handler
	{
	public:
		FileHandler(const std::string& baseFileName);

	private:
		virtual void utilizeLog(const LogRecord& logRecord);

		std::ofstream mLogFile;
	};
}
