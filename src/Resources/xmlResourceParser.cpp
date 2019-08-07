#include "xmlResourceParser.hpp"
#include "Utilities/xml.hpp"
#include "gameData.hpp"

namespace ph {

void XmlResourceParser::parseFile(GameData* const gameData, const std::string& filePath)
{
	PH_LOG_INFO("NeededResources file (" + filePath + ") is beign parsed.");

	Xml neededResourcesFile;
	neededResourcesFile.loadFromFile(filePath);
	const Xml loadingNode = *neededResourcesFile.getChild("loading");
	const Xml textureNode = *loadingNode.getChild("textures");
	loadTextures(textureNode, gameData);
}

void XmlResourceParser::loadTextures(const Xml& textureNode, GameData* const gameData)
{
	const std::vector<Xml> textureNodes = textureNode.getChildren("resource");
	for(const auto& node : textureNodes) {
		const std::string filename = node.getAttribute("respath").toString();
		gameData->getTextures().load("textures/" + filename);
	}
}

}