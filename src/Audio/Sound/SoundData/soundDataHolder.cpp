#include "soundDataHolder.hpp"

#include "soundData.hpp"

ph::SoundDataHolder::SoundDataHolder()
{
	using namespace std::string_literals;
	mAllSoundsData["sounds/barretaShot.wav"s] = SoundData(1.4f);
	mAllSoundsData["sounds/zombieGetsAttacked.wav"s] = SoundData(5.f, false, 200.f, 1000.f);
}

auto ph::SoundDataHolder::getSoundData(const std::string& filePath) -> SoundData
{
	const auto found = mAllSoundsData.find(filePath);
	return found->second;
}