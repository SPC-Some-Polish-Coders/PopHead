#pragma once

#include "ECS/system.hpp"

namespace ph::component {
	struct ParticleEmitter;
	struct BodyRect;
}

namespace ph::system {

class PatricleSystem : public System
{
public:
	using System::System;

	void update(float dt) override;

private:
	void updateParticleEmitter(float dt, ph::component::ParticleEmitter&, const ph::component::BodyRect&) const;
};

}