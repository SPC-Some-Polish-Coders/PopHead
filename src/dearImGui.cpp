#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/System/Clock.hpp>

bool debugWindowOpen = false; 
// NOTE: It isn't static for purpose. We access it through extern specifier in other translation units.

namespace ph {

struct FPSCounter
{
	sf::Clock clock;
	unsigned fps;
	unsigned framesFromLastSecond;
};

static FPSCounter fpsCounter;
static bool mouseJustPressed[5];

static const char* getClipboardText(void* user_data)
{
	return reinterpret_cast<const char*>(sf::Clipboard::getString().getData());
}

static void setClipboardText(void* user_data, const char* text)
{
	sf::Clipboard::setString(text);
}

static void initImGui(sf::Window& window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
	io.BackendPlatformName = "ph_dear_imgui_impl";

	io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
    io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
    io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
    io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
    io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
    io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
    io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
    io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
    io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
    io.KeyMap[ImGuiKey_Insert] = sf::Keyboard::Insert;
    io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
    io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
    io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Space;
    io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
    io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
    io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
    io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
    io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
    io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
    io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
    io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

    io.SetClipboardTextFn = setClipboardText;
    io.GetClipboardTextFn = getClipboardText;

    io.ImeWindowHandle = (void*)window.getSystemHandle();

	// TODO: Cursors

	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
}

static void shutdownImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
}

static void beginImGui(sf::Window& window, float dt)
{
	ImGui_ImplOpenGL3_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	io.DeltaTime = dt;

	// setup display size
	sf::Vector2u windowSize = window.getSize();
	io.DisplaySize = ImVec2(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
	io.DisplayFramebufferScale = ImVec2(1.f, 1.f);

	// update buttons
	for(int i = 0; i < IM_ARRAYSIZE(io.MouseDown); ++i)
    {
        io.MouseDown[i] = mouseJustPressed[i] || sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
        mouseJustPressed[i] = false;
    }

	// Update mouse position
	if(io.WantSetMousePos)
	{	
		sf::Vector2i mousePos(static_cast<int>(io.MousePos.x), static_cast<int>(io.MousePos.y));
		sf::Mouse::setPosition(mousePos, window);
	}
	else
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	}

	ImGui::NewFrame();

	if(debugWindowOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
		ImGui::Begin("Debug & Editor");

		if(fpsCounter.clock.getElapsedTime().asSeconds() >= 1.f)
		{
			fpsCounter.fps = fpsCounter.framesFromLastSecond;
			fpsCounter.clock.restart();
			fpsCounter.framesFromLastSecond = 0;
		}
		else
		{	
			++fpsCounter.framesFromLastSecond;
		}
		ImGui::Text("FPS: %u", fpsCounter.fps);
	
		ImGui::BeginTabBar("tab bar");
	}
}

static void endImGui()
{
	if(debugWindowOpen)
	{
		ImGui::EndTabBar();
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void imGuiHandleEvents(sf::Event e)
{
	ImGuiIO& io = ImGui::GetIO();

	switch(e.type)
	{
		case sf::Event::MouseWheelScrolled:
		{
			if(e.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel ||
			  (e.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel && 
			   io.KeyShift)) 
			{
				io.MouseWheel += e.mouseWheelScroll.delta;
			} 
			else if(e.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) 
			{
				io.MouseWheelH += e.mouseWheelScroll.delta;
			}
		} break;

		case sf::Event::KeyPressed:
		{
			if(e.key.code == sf::Keyboard::F5)
			{
				debugWindowOpen = !debugWindowOpen;
			}
		}
		case sf::Event::KeyReleased:
		{
			io.KeysDown[e.key.code] = e.type == sf::Event::KeyPressed;
		} break;

		case sf::Event::TextEntered:
		{
			if(e.text.unicode >= ' ' && e.text.unicode != 127)
			{
				io.AddInputCharacter(e.text.unicode);
			}
		} break;
	}
}

}
