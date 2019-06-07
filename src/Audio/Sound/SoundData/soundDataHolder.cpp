#include "soundDataHolder.hpp"

using PopHead::Audio::SoundDataHolder;
using PopHead::Audio::SoundData;

SoundDataHolder::SoundDataHolder()
{
	using namespace std::string_literals;
	mAllSoundsData["sounds/barretaShot.wav"s] = SoundData(1.4f);
	mAllSoundsData["sounds/zombieGetsAttacked.wav"s] = SoundData(5.f, false, 200.f, 1000.f);
}

SoundData SoundDataHolder::getSoundData(const std::string& filePath)
{
	const auto found = mAllSoundsData.find(filePath);
	return found->second;
}