#include "logsInitializing.hpp"

#include "ConcreteHandlers/consoleHandler.hpp"
#include "ConcreteHandlers/fileHandler.hpp"
#include "ConcreteHandlers/terminalHandler.hpp"

#include "logger.hpp"

#include "Utilities/xml.hpp"

namespace ph {

	void initializeLogsModule(const std::string& configFileName)
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
			//else if (type == "terminalHandler")

			if (!handler)
				continue;

			auto modules = handlerTag.getChild("modules").getChildren("module");

			for (const auto& moduleTag : modules)
			{
				std::string moduleName = moduleTag.toString();
				handler->setModuleAllowing(moduleName, true);
			}

			auto types = handlerTag.getChild("types").getChildren("typeT");

			for (const auto& typeTag : types)
			{
				std::string type = typeTag.toString();

				if (type == "Info")
					handler->setTypeAllowing(LogType::Info, true);
				if (type == "Error")
					handler->setTypeAllowing(LogType::Error, true);
				if (type == "Warning")
					handler->setTypeAllowing(LogType::Warning, true);
				if (type == "FromUser")
					handler->setTypeAllowing(LogType::FromUser, true);
				if (type == "Exception")
					handler->setTypeAllowing(LogType::Exception, true);
				if (type == "UnhandledException")
					handler->setTypeAllowing(LogType::UnhandledException, true);
			}

			Logger::addLogsHandler(std::move(handler));
		}
	}
}
