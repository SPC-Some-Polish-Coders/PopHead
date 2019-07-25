#include "spawn.hpp"

template<typename T>
void ph::Spawn::spawn()
{
	auto& creature = std::make_unique<T>(mGameData);
	auto& root = getRoot();
	creature->setPosition(mSpawnPosition);
	root.addChild(std::move(creature));
}
