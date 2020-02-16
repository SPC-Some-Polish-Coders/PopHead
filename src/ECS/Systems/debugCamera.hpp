#include "ECS/system.hpp"

namespace ph::system {

class DebugCamera : public System
{
public:
	using System::System;

	void onEvent(sf::Event) override;
	void update(float dt) override;

private:
	bool mIsHintActive = true;
};

}

