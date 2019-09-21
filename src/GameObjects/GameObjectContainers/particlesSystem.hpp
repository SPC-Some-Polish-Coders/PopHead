#pragma once

#include "GameObjects/gameObject.hpp"
#include <vector>

namespace ph {

class Renderer;
class Particles;

class ParticlesSystem : public GameObject
{
public:
	ParticlesSystem(Renderer& renderer);

	void updateCurrent(const sf::Time delta) override;
	void addParticlesToDelete(Particles* deletingParticles);

private:
	void handleParticlesToDelete();

private:
	Renderer& mRenderer;
	std::vector<Particles*> mParticlesToDelete;
};

}