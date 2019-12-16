#pragma once

#include "Logs/handler.hpp"

namespace ph {

	class MockHandler : public Handler
	{
		virtual void utilizeLog(const LogRecord& logRecord) override {}
	};
}
