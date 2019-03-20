#ifndef POPHEAD_STATES_STATEMACHINE_H_
#define POPHEAD_STATES_STATEMACHINE_H_

#include <memory>
#include <vector>
#include <SFML/System.hpp>

#include "State.hpp"

namespace PopHead {
    namespace Base { class GameData; }
namespace States {

using StatePtr = std::unique_ptr<State>;


class StateMachine
{
public:
    StateMachine();

    void changingStatesProcess();
    void input();
    void update(sf::Time delta);
    void pushState(StatePtr state);
    void replaceState(StatePtr replacer);
    void popState();
    void clearStates();

    unsigned int getStatesAmount() const;
    bool getHideInStateNr(unsigned int nrOfState) const;
    bool getPauseInStateNr(unsigned int nrOfState) const;

    void setHideInStateNr(unsigned int nrOfState, bool hide);
    void setPauseInStateNr(unsigned int nrOfState, bool pause);

private:
    std::vector<StatePtr> mActiveStates;
    std::vector<StatePtr> mPendingStates;

    bool mIsAdding;
    bool mIsReplacing;
    bool mIsRemoving;
    bool mIsClearing;
};


}}

#endif // !POPHEAD_STATES_STATEMACHINE_H_
