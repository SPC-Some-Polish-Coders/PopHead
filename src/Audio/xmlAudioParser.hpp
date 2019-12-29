#pragma once

#include <string>

namespace ph {

class Xml;
class SoundPlayer;
class MusicPlayer;

class XmlAudioParser
{
public:
	void parseFile(SoundPlayer& soundPlayer, MusicPlayer& musicPlayer, const std::string& filePath);

private:
	void parseSoundMute(const Xml& audioNode);
	void parseStartTheme(const Xml& audioNode);
	void parseMusicStates(const Xml& audioNode);

private:
	SoundPlayer* mSoundPlayer;
	MusicPlayer* mMusicPlayer;
};

}
