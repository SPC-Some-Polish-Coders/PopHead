#include "GameObjects/GameObjectContainers/particlesSystem.hpp"
#include "GameObjects/DrawableGameObjects/particles.hpp"
#include "gameData.hpp"

namespace ph {

ParticlesSystem::ParticlesSystem(GameData* const gameData)
	:mGameData(gameData)
	,GameObject("particlesSystem")
{
}

void ParticlesSystem::handleParticlesToDelete()
{
	for (Particles* particles : mParticlesToDelete)
	{
		mGameData->getRenderer().removeDrawableGameObject(particles);
		removeChild(particles);
	}
		mParticlesToDelete.clear();
}

void ParticlesSystem::update(const sf::Time delta)
{
	GameObject::update(delta);
	handleParticlesToDelete();
}

}