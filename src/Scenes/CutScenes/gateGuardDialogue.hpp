//#pragma once
//
//#include "Scenes/cutScene.hpp"
//
//#include "entt/entity/registry.hpp"
//
//namespace ph {
//
//class GUI;
//
//class GateGuardDialogue : public CutScene
//{
//public:
//	GateGuardDialogue(entt::registry& gameRegistry, GUI& gui);
//
//	void update(const sf::Time dt) override;
//
//private:
//	void initGui();
//	void leaveCutScene();
//
//private:
//	entt::registry& mRegistry;
//	GUI& mGui;
//	sf::Clock mTimeSinceLastSkipPress;
//	int mTimesPressedSkip;
//	bool mPlayerOnThePosition;
//};
//
//}