#pragma once

#include "Renderer/renderer.hpp"
#include "Renderer/MinorRenderers/quadData.hpp"
#include <vector>

#define MORE_THEN_32_TEXTURES 1

namespace ph {

void rendererTest()
{
	/* 1  */ static Texture awsdTex2("resources/textures/others/awsdHint.png");
	/* 2  */ static Texture backslashTex2("resources/textures/others/backslashHint.png");
	/* 3  */ static Texture barTex2("resources/textures/others/bar.png");
	/* 4  */ static Texture baseballTex2("resources/textures/others/baseballStick.png");
	/* 5  */ static Texture bulletTex2("resources/textures/others/bullet.png");
	/* 6  */ static Texture bulletGroundTex2("resources/textures/others/bulletGround.png");
	/* 7  */ static Texture enterTex2("resources/textures/others/enterHint.png");
	/* 8  */ static Texture gateTex2("resources/textures/others/gate.png");
	/* 9  */ static Texture heartTex2("resources/textures/others/heart.png");
	/* 10 */ static Texture leverTex2("resources/textures/others/lever.png");
	/* 11 */ static Texture medkitTex2("resources/textures/others/medkit.png");
	/* 12 */ static Texture pistolTex2("resources/textures/others/pistol.png");
	/* 13 */ static Texture awsdTex("resources/textures/others/awsdHint.png");
	/* 14 */ static Texture backslashTex("resources/textures/others/backslashHint.png");
	/* 15 */ static Texture barTex("resources/textures/others/bar.png");
	/* 16 */ static Texture baseballTex("resources/textures/others/baseballStick.png");
	/* 17 */ static Texture bulletTex("resources/textures/others/bullet.png");
	/* 18 */ static Texture bulletGroundTex("resources/textures/others/bulletGround.png");
	/* 19 */ static Texture enterTex("resources/textures/others/enterHint.png");
	/* 20 */ static Texture gateTex("resources/textures/others/gate.png");
	/* 21 */ static Texture heartTex("resources/textures/others/heart.png");
	/* 22 */ static Texture leverTex("resources/textures/others/lever.png");
	/* 23 */ static Texture medkitTex("resources/textures/others/medkit.png");
	/* 24 */ static Texture pistolTex("resources/textures/others/pistol.png");
	/* 25 */ static Texture spaceTex("resources/textures/others/pressSpaceHint.png");
	/* 26 */ static Texture zombieHeadTex("resources/textures/others/zombieHead.png");
	/* 27 */ static Texture arcadeCounBackTex("resources/textures/gui/arcadeCountersBackground.png");
	/* 28 */ static Texture arenaButtonTex("resources/textures/gui/arenaButton.png");
	/* 29 */ static Texture bunkerTex("resources/textures/gui/bunker.jpg");
	/* 30 */ static Texture creditsButtonTex("resources/textures/gui/creditsButton.png");
	/* 31 */ static Texture exitButtonTex("resources/textures/gui/exitButton.png");
	/* 32 */ static Texture mirrorSpTex("resources/textures/gui/mirrorSpeechBubble.png");
	/* 33 */ static Texture optionsButtonTex("resources/textures/gui/optionsButton.png");
	/* 34 */ static Texture sewageTex("resources/textures/gui/sewage.jpg");

	Renderer::submitQuad(&medkitTex2, nullptr, nullptr, nullptr, sf::Vector2f(-1500.f, 0.f), sf::Vector2f(3000.f, 3000.f), 100, 0.f, false);

	for(int i = 0; i < 2500; ++i) {
		Renderer::submitQuad(&awsdTex2, nullptr, nullptr, nullptr, sf::Vector2f(-550.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 45.f, false);
		Renderer::submitQuad(&backslashTex2, nullptr, &sf::Color::Red, nullptr, sf::Vector2f(-500.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 123.f, false);
		Renderer::submitQuad(&barTex2, nullptr, nullptr, nullptr, sf::Vector2f(-450.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&baseballTex2, nullptr, nullptr, nullptr, sf::Vector2f(-400.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&bulletTex2, nullptr, nullptr, nullptr, sf::Vector2f(-350.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&bulletGroundTex2, nullptr, nullptr, nullptr, sf::Vector2f(-300.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&enterTex2, &IntRect(5, 5, 10, 10), nullptr, nullptr, sf::Vector2f(-250.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&gateTex2, nullptr, nullptr, nullptr, sf::Vector2f(-200.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10,0.f, false);
		Renderer::submitQuad(&heartTex2, nullptr, nullptr, nullptr, sf::Vector2f(-150.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10,0.f, false);
		Renderer::submitQuad(&leverTex2, nullptr, nullptr, nullptr, sf::Vector2f(-100.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10,0.f, false);
		Renderer::submitQuad(&medkitTex2, nullptr, nullptr, nullptr, sf::Vector2f(-50.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&pistolTex2, nullptr, nullptr, nullptr, sf::Vector2f(0.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&awsdTex, nullptr, nullptr, nullptr, sf::Vector2f(50.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&backslashTex, nullptr, nullptr, nullptr, sf::Vector2f(100.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&barTex, nullptr, &sf::Color::Yellow, nullptr, sf::Vector2f(150.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&baseballTex, nullptr, nullptr, nullptr, sf::Vector2f(200.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&bulletTex, nullptr, nullptr, nullptr, sf::Vector2f(250.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&bulletGroundTex, nullptr, nullptr, nullptr, sf::Vector2f(300.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&enterTex, nullptr, nullptr, nullptr, sf::Vector2f(350.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
#if MORE_THEN_32_TEXTURES
		Renderer::submitQuad(&gateTex, nullptr, nullptr, nullptr, sf::Vector2f(400.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&heartTex, nullptr, nullptr, nullptr, sf::Vector2f(450.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&leverTex, nullptr, nullptr, nullptr, sf::Vector2f(500.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&medkitTex, nullptr, nullptr, nullptr, sf::Vector2f(550.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&pistolTex, nullptr, nullptr, nullptr, sf::Vector2f(600.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&spaceTex, nullptr, nullptr, nullptr, sf::Vector2f(650.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&zombieHeadTex, nullptr, nullptr, nullptr, sf::Vector2f(700.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&arcadeCounBackTex, nullptr, nullptr, nullptr, sf::Vector2f(750.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&arenaButtonTex, nullptr, nullptr, nullptr, sf::Vector2f(800.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&bunkerTex, nullptr, nullptr, nullptr, sf::Vector2f(850.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&creditsButtonTex, nullptr, nullptr, nullptr, sf::Vector2f(900.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&exitButtonTex, nullptr, nullptr, nullptr, sf::Vector2f(950.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&sewageTex, nullptr, nullptr, nullptr, sf::Vector2f(1000.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&mirrorSpTex, nullptr, nullptr, nullptr, sf::Vector2f(1050.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
		Renderer::submitQuad(&optionsButtonTex, nullptr, nullptr, nullptr, sf::Vector2f(1100.f, ( float) i * 50.f), sf::Vector2f(20.f, 30.f), 10, 0.f, false);
#endif
	}
}

void pointTest()
{
	for(float i = 0; i < 10; ++i)
		Renderer::submitPoint({10.f * i - 200.f, 10.f * i}, sf::Color(char(i * 20), char(i * 10), char(i * 5)), 0, 10.f);
}

void lineTest()
{
	Renderer::submitLine(sf::Color::Red, {0.f, 0.f}, {50.f, 50.f}, 5.f);
	Renderer::submitLine(sf::Color::Red, {15.f, 0.f}, {15.f, 50.f});
}

void submitBunchOfQuadsWithTheSameTextureTest()
{
	static Texture texture("resources/textures/others/gate.png");

	static std::vector<QuadData> quads;
	for(int x = 0; x < 100; ++x)
		for(int y = 0; y < 100; ++y)
		{
			QuadData qd;
			qd.position = {x * 20.f, y * 20.f};
			qd.rotation = 0.f;
			qd.size = {20.f, 20.f};
			qd.textureRect = FloatRect(0.f, 0.f, 1.f, 1.f);
			qd.textureSlotRef = 0.f;
			qd.color = Vector4f{1.f, 1.f, 1.f, 1.f};

			quads.emplace_back(qd);
		}

	Renderer::submitBunchOfQuadsWithTheSameTexture(quads, &texture, nullptr, 200);

	quads.clear();
}

}