#ifndef POPHEAD_WORLD_ANIMATION_ANIMATION_H_
#define POPHEAD_WORLD_ANIMATION_ANIMATION_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>

namespace PopHead {
namespace World{


class Animation
{
public:
	Animation() = default;

	Animation(const sf::Time& delay);

	void setDelay(const sf::Time& delay) { mDelay = delay; }

	void addState(const std::string& stateName, const std::vector<sf::IntRect>& frames);

	void addState(const std::string& stateName, sf::IntRect frame, unsigned framesCount);

	void changeState(const std::string& stateName);

	void animate(sf::Sprite& sprite);

	void animate(sf::Sprite& sprite, const sf::Time& deltaTime);

	void goToFrontFrame() { mCurrentFrameIndex = 0; };

	auto getCurrentStateName() const -> std::string;

private:
	sf::Time mDelay = sf::seconds(0.1f);
	sf::Time mElapsedTime = sf::Time::Zero;
	std::map<std::string, std::vector<sf::IntRect>> mStates;
	std::string mCurrentStateName;
	std::size_t mCurrentFrameIndex = 0;
};


}}

#endif
