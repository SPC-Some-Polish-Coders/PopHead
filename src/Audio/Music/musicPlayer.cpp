#include "musicPlayer.hpp"

namespace ph {

MusicPlayer::MusicPlayer()
	:mVolume(70.f)
	,mIsMuted(false)
	,mCurrentThemeFilePath()
{
}

MusicPlayer::~MusicPlayer()
{
	mMusic.stop();
}

void MusicPlayer::play(const std::string& filePath)
{
	//if(filePath == mCurrentThemeFilePath)
		//return;
	mCurrentThemeFilePath = filePath;

	const MusicData currentThemeData = mMusicDataHolder.getMusicData(filePath);
	const std::string fullFilePath = "resources/" + filePath;

	mMusic.openFromFile(fullFilePath);
	auto multiplier = getMultiplier(filePath);
	auto volume = mVolume * currentThemeData.mVolumeMultiplier * multiplier;
	mMusic.setVolume(volume);
	mMusic.setLoop(currentThemeData.mLoop);
	setMuted(mIsMuted);
	mMusic.play();
}

void MusicPlayer::stop()
{
	mMusic.stop();
}

void MusicPlayer::setVolumeMultiplierForTheme(const std::string& theme, float multiplier)
{
	mThemesVolumeMultipliers[theme] = multiplier;
}

void MusicPlayer::setPaused(const bool pause)
{
	pause ? mMusic.pause() : mMusic.play();
}

void MusicPlayer::setMuted(const bool mute)
{
	mute ? mMusic.setVolume(0.f) : setVolume(mVolume);
	mIsMuted = mute;
}

void MusicPlayer::setVolume(const float volume)
{
	mVolume = volume;
	const auto themeData = mMusicDataHolder.getCurrentThemeData();
	const float volumeMultiplier = themeData.mVolumeMultiplier;
	mMusic.setVolume(volume * volumeMultiplier * getMultiplier(mCurrentThemeFilePath));
}

float MusicPlayer::getMultiplier(const std::string& theme) const
{
	auto iter = mThemesVolumeMultipliers.find(theme);
	if (iter == mThemesVolumeMultipliers.end())
		return 1.f;
	return iter->second;
}

}