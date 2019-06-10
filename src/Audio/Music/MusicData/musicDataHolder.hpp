#ifndef POPHEAD_AUDIO_MUSICDATAHOLDER_H_
#define POPHEAD_AUDIO_MUSICDATAHOLDER_H_

#include "musicData.hpp"
#include <string>
#include <map>

namespace PopHead {
namespace Audio {

class MusicDataHolder
{
public:
	MusicDataHolder();

	auto getMusicData (const std::string& filePath) -> const MusicData&;
	auto getCurrentThemeData() -> const MusicData& { return mCurrentThemeData; }

private:
	std::map<std::string, MusicData> mAllThemesData;
	MusicData mCurrentThemeData;
};

}}


#endif // !POPHEAD_AUDIO_MUSICDATAHOLDER_H_

