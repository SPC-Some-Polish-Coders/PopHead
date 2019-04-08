#include "animation.hpp"
#include <stdexcept>

using PopHead::World::Animation;

Animation::Animation(const sf::Time& delay)
    : mDelay(delay)
{
}

void Animation::addState(const std::string& stateName, const std::vector<sf::IntRect>& frames)
{
	if (frames.size() == 0)
		throw std::runtime_error(stateName + " state doesn't contain any frames");
	auto result = mStates.insert(std::pair<std::string, std::vector<sf::IntRect>>(stateName, frames));
	if (!result.second)
		throw std::runtime_error(stateName + " state already exists in animation");
	if (mStates.size() == 1)
		mCurrentStateName = stateName;
}

void Animation::addState(const std::string & stateName, sf::IntRect frame, unsigned framesCount)
{
	if (framesCount == 0)
		throw std::runtime_error("Frames count must be greater than 0");
	std::vector<sf::IntRect> frames(framesCount);
	for (unsigned i = 0; i < framesCount; i++) {
		frames[i] = frame;
		frame.left += frame.width;
	}
	auto result = mStates.insert(std::pair<std::string, std::vector<sf::IntRect>>(stateName, frames));
	if (!result.second)
		throw std::runtime_error(stateName + " state already exists in animation");
	if (mStates.size() == 1)
		mCurrentStateName = stateName;
}

void Animation::changeState(const std::string & stateName)
{
	auto result = mStates.find(stateName);
	if (result == mStates.end())
		throw std::runtime_error(stateName + " state doesn't exist in animation");
	mCurrentStateName = stateName;
	mCurrentFrameIndex = 0;
}

void Animation::animate(sf::Sprite & sprite)
{
	if (mStates.empty())
		throw std::runtime_error("Add at least one state to animate");
	const sf::IntRect frame = mStates[mCurrentStateName][mCurrentFrameIndex];
	sprite.setTextureRect(frame);
	if (++mCurrentFrameIndex == mStates[mCurrentStateName].size())
		mCurrentFrameIndex = 0;
}

void Animation::animate(sf::Sprite & sprite, const sf::Time & deltaTime)
{
	if (mStates.empty())
		throw std::runtime_error("Add at least one state to animate");
	mElapsedTime += deltaTime;
	while (mElapsedTime >= mDelay) {
		mElapsedTime -= mDelay;
		const sf::IntRect frame = mStates[mCurrentStateName][mCurrentFrameIndex];
		sprite.setTextureRect(frame);
		if (++mCurrentFrameIndex == mStates[mCurrentStateName].size())
			mCurrentFrameIndex = 0;
	}
}

std::string Animation::getCurrentStateName() const
{
	if (mStates.empty())
		throw std::runtime_error("Add at least one state to get current state name");
	return mCurrentStateName;
}
