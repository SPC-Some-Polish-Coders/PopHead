#pragma once

#include <memory>
#include <vector>
#include <deque>
#include <SFML/System.hpp>

#include "States/state.hpp"

namespace ph {

class GameData;

using StatePtr = std::unique_ptr<State>;

class StateMachine
{
public:
    StateMachine();

    void pushState();
    void replaceState();
    void popState();
    void clearStates();
    
	void changingStatesProcess();
private:
	void pushAction();
	void replaceAction();
	void popAction();
	void clearAction();

public:
    void input();
    void update(sf::Time delta);

	State& getTopState() { return *(mActiveStates.back().get()); }

    auto getStatesAmount() const -> unsigned int {return mActiveStates.size();}
    bool getHideInStateNr(unsigned int nrOfState) const; /// 0 is top
    bool getPauseInStateNr(unsigned int nrOfState) const;
    void setHideInStateNr(unsigned int nrOfState, bool hide);
    void setPauseInStateNr(unsigned int nrOfState, bool pause);

    void setGameData( ph::GameData* const gameData ){mGameData = gameData;}

private:
    std::vector<StatePtr> mActiveStates;
    std::deque<StatePtr> mPendingStates;

    GameData* mGameData;

    bool mIsPushing;
    bool mIsReplacing;
    bool mIsPopping;
    bool mIsClearing;
};

}
