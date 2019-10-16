#include "musicStateMachine.hpp"
#include "Logs/logs.hpp"

namespace ph {

void MusicStateMachine::addState(std::string stateName, MusicState& state)
{
	mStates[stateName] = state;
}

auto MusicStateMachine::getRandomThemeFromState(const std::string& stateName) const -> std::pair<std::string, float>
{
	auto found = mStates.find(stateName);
	PH_ASSERT_CRITICAL(found != mStates.end(), "Music state \"" + stateName + "\" does not exist!");

	// TODO: Implement randomness
	return std::pair<std::string, float>(found->second.filepaths[0], found->second.volumeMultipliers[0]);
}

void MusicStateMachine::clearStates() noexcept
{
	mStates.clear();
}

}
