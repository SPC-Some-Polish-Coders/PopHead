#pragma once

#include <string>
#include <map>
#include <vector>

namespace ph {

struct MusicState
{
	std::vector<std::string> filepaths;
	std::vector<float> volumeMultipliers;
};

class MusicStateMachine
{
public:
	void addState(std::string stateName, MusicState& states);
	auto getRandomThemeFromState(const std::string& stateName) const -> std::pair<std::string, float>;
	bool hasMusicState(const std::string& musicStateName) const;
	void clearStates() noexcept;

private:
	std::map<std::string, MusicState> mStates;
};

}
