#include "cutScene.hpp"

namespace ph {

CutScene::CutScene(GameObject& root)
	: mRoot(root)
	, mIsActive(true)
{
}

bool CutScene::isActive() const
{
	return mIsActive;
}

}

