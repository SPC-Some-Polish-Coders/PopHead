template<std::size_t SIZE>
ph::Animation::Animation(
	const std::array<std::string, SIZE>& statesNames,
	const std::array<std::vector<sf::IntRect>, SIZE>& statesFrames,
	const sf::Time& delay)
	:mDelay(delay)
{
	for (std::size_t i = 0; i < SIZE; ++i)
		addState(statesNames[i], statesFrames[i]);
}

template<std::size_t SIZE>
ph::Animation::Animation(
	const std::array<std::string, SIZE> statesNames,
	std::array<sf::IntRect, SIZE> statesFrames,
	std::array<unsigned, SIZE> framesCounts,
	const sf::Time & delay)
	:mDelay(delay)
{
	for (std::size_t i = 0; i < SIZE; ++i)
		addState(statesNames[i], statesFrames[i], framesCounts[i]);
}
