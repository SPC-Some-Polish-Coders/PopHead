template<typename T>
void ph::Spawn::spawn()
{
	auto& creature = std::make_unique<T>(mGameData);
	auto& root = getRoot();
	creature->setPosition(mSpawnPosition);
	root.addChild(std::move(creature));
}

template<typename T>
void ph::Spawn::spawnEnemy()
{
	auto& enemy = std::make_unique<T>(mGameData);
	auto& root = getRoot();
	auto& enemies = root.getChild("enemy_container");
	enemy->setPosition(mSpawnPosition);
	enemies.addChild(std::move(enemy));
}
