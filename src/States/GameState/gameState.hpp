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

    void input() override;
    void update(sf::Time delta) override;

private:
    //sf::Texture mTexture; ///this is very temporary later it'll be handled by Resources module
};


}}

#endif // !POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
