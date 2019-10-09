#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>
#include <map>

namespace ph {

struct VertexBuffer;

class Animation
{
public:
	Animation() = default;

	Animation(const sf::Time& delay);

	template <std::size_t SIZE>
	Animation(const std::array<std::string, SIZE>& statesNames,
		const std::array<std::vector<sf::IntRect>, SIZE>& statesFrames,
		const sf::Vector2i textureSize,
		const sf::Time& delay = sf::seconds(0.1f));

	template <std::size_t SIZE>
	Animation(const std::array<std::string, SIZE> statesNames,
		std::array<sf::IntRect, SIZE> statesFrames,
		std::array<unsigned, SIZE> framesCounts,
		const sf::Vector2i textureSize,
		const sf::Time& delay = sf::seconds(0.1f));

	void setDelay(const sf::Time& delay) { mDelay = delay; }

	void addState(const std::string& stateName, const std::vector<sf::IntRect>& frames);

	void addState(const std::string& stateName, sf::IntRect frame, unsigned framesCount);

	void changeState(const std::string& stateName);

	void animate(const VertexBuffer&);

	void animate(const VertexBuffer&, const sf::Time& deltaTime);

	void goToFrontFrame();

	std::string getCurrentStateName() const;

private:
	std::map<std::string, std::vector<sf::IntRect>> mStates;
	std::string mCurrentStateName;
	sf::Vector2i mTextureSize;
	sf::Time mDelay = sf::seconds(0.1f);
	sf::Time mElapsedTime = sf::Time::Zero;
	std::size_t mCurrentFrameIndex = 0;
};

#include "animation.inl"

}
