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
	void updateSingleParticleEmitters(const float dt) const;
	void updateMultiParticleEmitters(const float dt) const;
	void updateParticleEmitter(const float dt, ph::component::ParticleEmitter&, const ph::component::BodyRect&) const;
};

}