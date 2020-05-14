#pragma once

namespace ph::component {
	
	struct DebugName // @no-debugger
	{
		char name[50];
	};

	struct DenialArea // @no-debugger
	{
		enum Type {Collision, LightWall, All};
		Type type;
	};

	struct TeleportPoint
	{
		std::string name;
	};
}
