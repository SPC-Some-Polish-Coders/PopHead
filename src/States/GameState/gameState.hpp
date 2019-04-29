#ifndef POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
#define POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP

#include "States/state.hpp"
#include <SFML/Graphics.hpp>

namespace PopHead{
namespace States{


class GameState : public State
{
public:
    GameState(Base::GameData *const);
private:
    void loadResources();
    void makeSceneTree();
    void makePlayer();
    void makeBoat();
    void makeNPC();
    void makeMap();
    void setCamera();
	void makeStaticObjectToCamera();

public:
    void input() override;

public:
    void update(sf::Time delta) override;
private:
	void cameraShake();
	void cameraMovement(sf::Time delta) const;
	void boatMovement(sf::Time delta);

private:
    ///this is very temporary later it'll be handled by Resources module
    sf::Texture mPlayerTexture;
    sf::Texture mMapTexture;
    sf::Texture mBoatTexture;
    sf::Texture mNPCTexture;

	bool shouldCameraShake = false;
};


}}

#endif // !POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
