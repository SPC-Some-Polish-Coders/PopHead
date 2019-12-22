#pragma once

#include "ECS/system.hpp"

#include "Utilities/rect.hpp"

namespace ph::system {

class Levers : public System
{
public:
	using System::System;

	void update(float dt) override;
	void onEvent(const ActionEvent& event) override;

private:
	void handleUsedLevers() const;
	void handleListeners(bool isActivated, unsigned leverId) const;

};

}