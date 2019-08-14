#include "GameObjects/GameObjectContainers/particlesSystem.hpp"
#include "GameObjects/DrawableGameObjects/particles.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

ParticlesSystem::ParticlesSystem(Renderer& renderer)
	:mRenderer(renderer)
	,GameObject("particlesSystem")
{
}

void ParticlesSystem::handleParticlesToDelete()
{
	for (Particles* particles : mParticlesToDelete) {
		mRenderer.removeDrawableGameObject(particles);
		removeChild(particles);
	}
	mParticlesToDelete.clear();
}

void ParticlesSystem::updateCurrent(const sf::Time delta)
{
	handleParticlesToDelete();
}

}