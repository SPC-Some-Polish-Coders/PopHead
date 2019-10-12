
namespace ph {

	template<typename SystemType, typename... Args>
	void SystemsQueue::appendSystem(Args... arguments)
	{
		mSystemsArray.emplace_back(std::unique_ptr<SystemType>(new SystemType(mRegistry, arguments...)));
	}

}
