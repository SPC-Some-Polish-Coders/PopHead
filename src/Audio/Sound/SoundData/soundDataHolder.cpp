#include "soundDataHolder.hpp"
#include "soundData.hpp"
#include "Utilities/xml.hpp"

namespace ph {

SoundDataHolder::SoundDataHolder()
{
	Xml soundDataXml;
	soundDataXml.loadFromFile("sounds/soundData.xml");
	const auto soundDataNode = *soundDataXml.getChild("soundData");
	const std::vector<Xml> soundNodes = soundDataNode.getChildren("sound");
	for(const auto& soundNode : soundNodes) {
		const std::string fileName = soundNode.getAttribute("fileName").toString();
		const std::string filePath = "sounds/" + fileName;
		const float volumeMultiplier = soundNode.getAttribute("volumeMultiplier").toFloat();
		const bool loop = soundNode.getAttribute("loop").toBool();
		const float maximalFullVolumeDistance = soundNode.getAttribute("maximalFullVolumeDistance").toFloat();
		const float maximalHearableDistance = soundNode.getAttribute("maximalHearableDistance").toFloat();
		mAllSoundsData[filePath] = SoundData(volumeMultiplier, loop, maximalFullVolumeDistance, maximalHearableDistance);
	}
}

auto SoundDataHolder::getSoundData(const std::string& filePath) -> SoundData
{
	const auto found = mAllSoundsData.find(filePath);
	return found->second;
}

}