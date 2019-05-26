#ifndef POPHEAD_RENDERER_RENDERERINILOADER_H_
#define POPHEAD_RENDERER_RENDERERINILOADER_H_

#include "Utilities/iniLoader.hpp"
#include "SFML/Graphics.hpp"

namespace PopHead{
namespace Renderer{

class WindowInitializer : private  Utilities::IniLoader
{
public:
	static sf::VideoMode iniGetWindowSize();
	static sf::Uint32 iniGetStyle();
private:
	static int iniGetWindowWidth();
	static int iniGetWindowHeight();
	static bool iniGetFullscreenMode();
};

}}
#endif // !POPHEAD_RENDERER_WINDOWINITIALIZER_H_
