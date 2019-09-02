#include "animation.hpp"
#include "Logs/logs.hpp"

namespace ph {

Animation::Animation(const sf::Time& delay)
    :mDelay(delay)
{
}

void Animation::addState(const std::string& stateName, const std::vector<sf::IntRect>& frames)
{
	PH_ASSERT(frames.size() > 0, " state doesn't contain any frames");
	auto result = mStates.insert(std::pair<std::string, std::vector<sf::IntRect>>(stateName, frames));
	PH_ASSERT(result.second, stateName + " state already exists in animation");
	if (mStates.size() == 1)
		mCurrentStateName = stateName;
}

void Animation::addState(const std::string& stateName, sf::IntRect frame, unsigned framesCount)
{
	PH_ASSERT(framesCount > 0, "Frames count must be greater than 0");
	std::vector<sf::IntRect> frames(framesCount);
	for (unsigned i = 0; i < framesCount; i++) {
		frames[i] = frame;
		frame.left += frame.width;
	}
	auto result = mStates.insert(std::pair<std::string, std::vector<sf::IntRect>>(stateName, frames));
	PH_ASSERT(result.second, stateName + " state already exists in animation");
	if (mStates.size() == 1)
		mCurrentStateName = stateName;
}

void Animation::changeState(const std::string& stateName)
{
	PH_ASSERT(mStates.find(stateName) != mStates.end(), stateName + " state doesn't exist in animation");
	mCurrentStateName = stateName;
	mCurrentFrameIndex = 0;
}

void Animation::animate(sf::Sprite& sprite)
{
	PH_ASSERT(!mStates.empty(), "Add at least one state to animate");
	const sf::IntRect frame = mStates[mCurrentStateName][mCurrentFrameIndex];
	sprite.setTextureRect(frame);
	if (++mCurrentFrameIndex == mStates[mCurrentStateName].size())
		mCurrentFrameIndex = 0;
}

void Animation::animate(sf::Sprite& sprite, const sf::Time& deltaTime)
{
	PH_ASSERT(!mStates.empty(), "Add at least one state to animate");
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
	PH_ASSERT(!mStates.empty(), "Add at least one state to get current state name");
	return mCurrentStateName;
}

}