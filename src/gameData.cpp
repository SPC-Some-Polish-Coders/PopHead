#include "gameData.hpp"

namespace ph {

void GameCloser::closeGame()
{
	mShouldGameBeClosed = true;
}

bool GameCloser::shouldGameBeClosed()
{
	return mShouldGameBeClosed;
}

}
