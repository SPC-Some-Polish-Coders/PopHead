#include "pch.hpp"
#include "handler.hpp"

namespace ph {
	
	Handler::Handler()
	{
		initializeLogLevels();
	}

	void Handler::handleLog(const LogRecord& logRecord)
	{
		if (isPassedByFilter(logRecord))
			utilizeLog(logRecord);
	}

	void Handler::setPathFilter(const std::string& path, bool allowed)
	{
		if (allowed)
			mAllowedPaths.emplace_back(path);
		else
			mDisallowedPaths.emplace_back(path);
	}

	void Handler::setLogLevelFilter(LogLevel level, bool allowed)
	{
		auto iter = std::find_if(mAllowedLogLevels.begin(), mAllowedLogLevels.end(), [&level](const std::pair<LogLevel, bool>& pair) {
			return pair.first == level;
			});

		if (iter != mAllowedLogLevels.end())
			iter->second = allowed;
	}

	bool Handler::isPathAllowed(const std::string& path) const
	{
		bool allowed = false;
		for (const auto& allowedPath : mAllowedPaths)
			if (path.find(allowedPath) != std::string::npos)
			{
				allowed = true;
				break;
			}

		if (allowed)
		{
			for (const auto& disallowedPath : mDisallowedPaths)
				if (path.find(disallowedPath) != std::string::npos)
				{
					allowed = false;
					break;
				}
		}

		return allowed;
	}

	bool Handler::isLogLevelAllowed(LogLevel level) const
	{
		auto iter = std::find_if(mAllowedLogLevels.begin(), mAllowedLogLevels.end(), [&level](const std::pair<LogLevel, bool>& pair) {
			return pair.first == level;
			});

		return iter != mAllowedLogLevels.end() && iter->second;
	}

	void Handler::enableAllPaths()
	{
		mAllowedPaths.clear();
		mDisallowedPaths.clear();
		mAllowedPaths.emplace_back(".");
		mAllowedPaths.emplace_back("/");
	}

	void Handler::disableAllPaths()
	{
		mAllowedPaths.clear();
		mDisallowedPaths.clear();
	}

	void Handler::enableAllLogLevels()
	{
		for (auto& level : mAllowedLogLevels)
			level.second = true;
	}

	bool Handler::isPassedByFilter(const LogRecord& logRecord) const
	{
		return isPathAllowed(logRecord.filePath) && isLogLevelAllowed(logRecord.level);
	}

	void Handler::initializeLogLevels()
	{
		auto levelsCount = Cast<std::size_t>(LogLevel::Count);
		mAllowedLogLevels.resize(levelsCount);

		for (std::size_t level = 0; level < levelsCount; ++level)
			mAllowedLogLevels.at(level) = std::make_pair(Cast<LogLevel>(level), false);
	}
}
