#pragma once

#include <string>

namespace ph {

class Xml;
class MusicPlayer;

class XmlAudioParser
{
public:
	void parseFile(MusicPlayer& musicPlayer, const std::string& filePath);

private:
	void parseSoundMute(const Xml& audioNode);
	void parseStartTheme(const Xml& audioNode);
	void parseMusicStates(const Xml& audioNode);

private:
	MusicPlayer* mMusicPlayer;
};

}
