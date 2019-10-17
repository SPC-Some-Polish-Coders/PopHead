template<typename T>
void ph::Spawn::spawn()
{
	auto creature = std::make_unique<T>(mGameData);
	creature->setPosition(mSpawnPosition);
	auto* standingObjects = getRoot().getChild("LAYER_standingObjects");
	standingObjects->addChild(std::move(creature));
}
