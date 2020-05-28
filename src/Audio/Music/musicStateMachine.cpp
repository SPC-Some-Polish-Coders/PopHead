#include "pch.hpp"
#include "musicStateMachine.hpp"
#include "Utilities/random.hpp"

namespace ph {

void MusicStateMachine::addState(std::string stateName, MusicState& state)
{
	mStates[stateName] = state;
}

auto MusicStateMachine::getRandomThemeFromState(const std::string& stateName) const -> std::pair<std::string, float>
{
	auto found = mStates.find(stateName);
	PH_ASSERT_CRITICAL(found != mStates.end(), "Music state \"" + stateName + "\" does not exist!");

	i32 numberOfThemes = Cast<i32>(found->second.filepaths.size());
	if(numberOfThemes == 1)
		return std::pair<std::string, float>(found->second.filepaths[0], found->second.volumeMultipliers[0]);
	else {
		const i32 indexOfThemeToPlay = Random::generateNumber(0, numberOfThemes - 1);
		return std::pair<std::string, float>(found->second.filepaths[indexOfThemeToPlay], found->second.volumeMultipliers[indexOfThemeToPlay]);
	}
}

bool MusicStateMachine::hasMusicState(const std::string& musicStateName) const
{
	return mStates.find(musicStateName) != mStates.cend();
}

void MusicStateMachine::clearStates() noexcept
{
	mStates.clear();
}

}
