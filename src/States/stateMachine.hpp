#ifndef POPHEAD_STATES_STATEMACHINE_H_
#define POPHEAD_STATES_STATEMACHINE_H_

#include <memory>
#include <vector>
#include <deque>
#include <SFML/System.hpp>

#include "state.hpp"
#include "stateIdentifiers.hpp"

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
    void pushState(StateID);
    void replaceState(StateID);
    void popState();
    void clearStates();

    unsigned int getStatesAmount() const;
    bool getHideInStateNr(unsigned int nrOfState) const; /// 0 is top
    bool getPauseInStateNr(unsigned int nrOfState) const;

    void setHideInStateNr(unsigned int nrOfState, bool hide);
    void setPauseInStateNr(unsigned int nrOfState, bool pause);

    // XXX not the best solution at all..
    void setGameData( Base::GameData* const );

private:
    auto getStatePtr(StateID) const -> std::unique_ptr<State>;

    std::vector<StatePtr> mActiveStates;
    std::deque<StatePtr> mPendingStates;

    Base::GameData* mGameData;

    bool mIsAdding;
    bool mIsReplacing;
    bool mIsRemoving;
    bool mIsClearing;
};

}}

#endif // !POPHEAD_STATES_STATEMACHINE_H_
