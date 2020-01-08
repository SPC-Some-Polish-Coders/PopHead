#include "musicData.hpp"
#include "Logs/logs.hpp"
#include "Utilities/xml.hpp"

namespace ph{

MusicData::MusicData(float volumeMultiplier, bool loop)
	:mVolumeMultiplier(volumeMultiplier)
	,mLoop(loop)
{
}

MusicDataHolder::MusicDataHolder()
{
	Xml musicDataXml;
	PH_ASSERT_CRITICAL(musicDataXml.loadFromFile("resources/music/musicData.xml"), "\"resources/music/musicData.xml\" wasn't loaded correctly!");
	const auto musicDataNode = musicDataXml.getChild("musicdata");
	const std::vector<Xml> themeNodes = musicDataNode->getChildren("theme");
	for(const auto& themeNode : themeNodes) {
		const std::string fileName = themeNode.getAttribute("filename")->toString();
		const std::string filePath = "music/" + fileName;
		const float volumeMultiplier = themeNode.getAttribute("volumemultiplier")->toFloat();
		const bool loop = themeNode.getAttribute("loop")->toBool();
		mAllThemesData[filePath] = MusicData(volumeMultiplier, loop);
	}
}

auto MusicDataHolder::getMusicData(const std::string& filePath) -> const MusicData&
{
	auto found = mAllThemesData.find(filePath);
	PH_ASSERT_CRITICAL(found != mAllThemesData.end(), "MusicData with filepath \"" + filePath + "\" was not found.");
	mCurrentThemeData = found->second;
	return found->second;
}

}

