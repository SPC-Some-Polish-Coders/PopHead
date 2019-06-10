#ifndef POPHEAD_RENDERER_RENDERERINILOADER_H_
#define POPHEAD_RENDERER_RENDERERINILOADER_H_

#include "Utilities/iniLoader.hpp"
#include "SFML/Graphics.hpp"

namespace PopHead{
namespace Renderer{

class WindowInitializer : private  Utilities::IniLoader
{
public:
	static sf::VideoMode getWindowSize();
	static sf::Uint32 getStyle();
private:
	static int getWindowWidth();
	static int getWindowHeight();
	static int getResolutionValue();
	static bool getFullscreenMode();
};

}}
#endif // !POPHEAD_RENDERER_WINDOWINITIALIZER_H_
