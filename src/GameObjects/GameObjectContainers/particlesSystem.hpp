#pragma once

#include "GameObjects/gameObject.hpp"
#include <vector>

namespace ph {

class GameData;
class Particles;

class ParticlesSystem : public GameObject
{
public:
	ParticlesSystem(GameData* const);

	void update(const sf::Time delta) override;
	void addParticlesToDelete(Particles* deletingParticles) { mParticlesToDelete.emplace_back(deletingParticles); }

private:
	void handleParticlesToDelete();

private:
	GameData* const mGameData;
	std::vector<Particles*> mParticlesToDelete;
};

}