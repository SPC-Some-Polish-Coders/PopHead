#include "GameObjects/DrawableGameObjects/particles.hpp"
#include "GameObjects/GameObjectContainers/particlesSystem.hpp"
#include "Utilities/random.hpp"
#include "gameData.hpp"

namespace ph {

Particles::Particles(Renderer& renderer, const sf::Vector2f startPosition)
	:DrawableGameObject(renderer, "particles", LayerID::airEntities)
	,mNumberOfParticles(150)
	,mLifetime(sf::milliseconds(250))
	,mPosition(startPosition)
	,mVertecies(sf::Points, mNumberOfParticles)
	,mParticles(mNumberOfParticles)
{
	initializeParticles();
}

void Particles::initializeParticles()
{
	for (size_t index = 0; index < mParticles.size(); ++index)
	{
		float angle = Random::generateNumber(1.f, 360.f);
		float speed = Random::generateNumber(50.f, 100.f); 
		mParticles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		mVertecies[index].color = sf::Color::Red;
		mVertecies[index].position = mPosition;
	}
}

void Particles::killParticles()
{
	auto particlesSystem = dynamic_cast<ParticlesSystem*>(mParent);
	particlesSystem->addParticlesToDelete(this);
}

void Particles::updateCurrent(const sf::Time delta)
{
	mLifetime -= delta;
	if (mLifetime < sf::Time::Zero)
		killParticles();

	for (size_t index = 0; index < mParticles.size(); ++index)
	{
		mPosition = mVertecies[index].position += mParticles[index].velocity * delta.asSeconds();
		setPosition(mPosition);
	}
}

void Particles::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mVertecies);
}

}