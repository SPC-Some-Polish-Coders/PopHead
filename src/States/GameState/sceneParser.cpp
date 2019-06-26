#include "States/GameState/sceneParser.hpp"

#include <iostream>


namespace ph {

SceneParser::SceneParser()
	:sourceName("scene.xml") {}

std::vector<std::string> SceneParser::loadResources()
{
	Xml sceneFile;
	sceneFile.loadFromFile(sourceName);
	std::vector<std::string> fileNames;
	loadTextures(fileNames, sceneFile);
	return fileNames;
}

void SceneParser::loadTextures(std::vector<std::string>& resVec, Xml& sceneFile)
{
	Xml textureNode = sceneFile.getChild("textures");
	std::vector<Xml> textureNodes = textureNode.getChildren("resource");
	for (auto& node : textureNodes)
	{
		const std::string filename = node.getAttribute("respath").toString();
		resVec.emplace_back("textures/" + filename);
	}
}

}