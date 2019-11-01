#include "cutScene.hpp"

namespace ph {

CutScene::CutScene()
	: mIsActive(true)
{
}

bool CutScene::isActive() const
{
	return mIsActive;
}

}

