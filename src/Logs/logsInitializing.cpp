#include "logsInitializing.hpp"

#include "ConcreteHandlers/consoleHandler.hpp"
#include "ConcreteHandlers/fileHandler.hpp"
#include "ConcreteHandlers/terminalHandler.hpp"

#include "logger.hpp"

#include "Utilities/xml.hpp"

namespace ph {

	void initializeLogsModule(const std::string& configFileName, Terminal* terminal)
	{
		Xml document;
		document.loadFromFile(configFileName);

		auto mainTag = document.getChild("handlers");
		auto handlers = mainTag.getChildren("handler");

		for (const auto& handlerTag : handlers)
		{
			std::string type = handlerTag.getAttribute("type").toString();

			std::unique_ptr<Handler> handler;
			if (type == "fileHandler")
				handler.reset(new FileHandler("logs\\log_"));
			else if (type == "consoleHandler")
				handler.reset(new ConsoleHandler());
			else if (type == "terminalHandler")
				handler.reset(new TerminalHandler(terminal));

			if (!handler)
				continue;

			auto modules = handlerTag.getChild("modules").getChildren("module");

			for (const auto& moduleTag : modules)
			{
				std::string moduleName = moduleTag.toString();
				handler->setModuleAllowing(moduleName, true);
			}

			auto logLevels = handlerTag.getChild("levels").getChildren("level");

			for (const auto& logLevel : logLevels)
			{
				std::string levelStr = logLevel.toString();

				if (levelStr == "Info")
					handler->setLogLevelAllowing(LogLevel::Info, true);
				if (levelStr == "Error")
					handler->setLogLevelAllowing(LogLevel::Error, true);
				if (levelStr == "Warning")
					handler->setLogLevelAllowing(LogLevel::Warning, true);
				if (levelStr == "Critical")
					handler->setLogLevelAllowing(LogLevel::Critical, true);
			}

			Logger::addLogsHandler(std::move(handler));
		}
	}
}
