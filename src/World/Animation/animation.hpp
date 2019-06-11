#ifndef POPHEAD_WORLD_ANIMATION_ANIMATION_H_
#define POPHEAD_WORLD_ANIMATION_ANIMATION_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>

namespace ph {

class Animation
{
public:
	Animation() = default;

	Animation(const sf::Time& delay);

	template <std::size_t SIZE>
	Animation(const std::array<std::string, SIZE>& statesNames,
		const std::array<std::vector<sf::IntRect>, SIZE>& statesFrames,
		const sf::Time& delay = sf::seconds(0.1f));

	template <std::size_t SIZE>
	Animation(const std::array<std::string, SIZE> statesNames,
		std::array<sf::IntRect, SIZE> statesFrames,
		std::array<unsigned, SIZE> framesCounts,
		const sf::Time& delay = sf::seconds(0.1f));

	void setDelay(const sf::Time& delay) { mDelay = delay; }

	void addState(const std::string& stateName, const std::vector<sf::IntRect>& frames);

	void addState(const std::string& stateName, sf::IntRect frame, unsigned framesCount);

	void changeState(const std::string& stateName);

	void animate(sf::Sprite& sprite);

	void animate(sf::Sprite& sprite, const sf::Time& deltaTime);

	void goToFrontFrame() { mCurrentFrameIndex = 0; };

	std::string getCurrentStateName() const;

private:
	std::map<std::string, std::vector<sf::IntRect>> mStates;
	std::string mCurrentStateName;
	sf::Time mDelay = sf::seconds(0.1f);
	sf::Time mElapsedTime = sf::Time::Zero;
	std::size_t mCurrentFrameIndex = 0;
};

#include "animation.inl"

}

#endif
