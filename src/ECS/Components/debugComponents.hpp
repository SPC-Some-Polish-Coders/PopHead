#pragma once

namespace ph::component {
	
	struct DebugName
	{
		char name[50];
	};

	struct DenialArea
	{
		enum Type {Collision, LightWall, All};
		Type type;
	};

	struct TeleportPoint
	{
		std::string name;
	};
}
