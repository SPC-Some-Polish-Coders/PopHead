#include "cutScene.hpp"

namespace ph {

CutScene::CutScene(bool pausesSystems)
	:mIsActive(true)
	,mPausesSystems(pausesSystems)
{
}

bool CutScene::isActive() const
{
	return mIsActive;
}

}

