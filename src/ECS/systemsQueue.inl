#include "systemsQueue.hpp"

namespace ph {

	template<typename SystemType, typename... Args>
	void SystemsQueue::appendSystem(Args... arguments)
	{
		if (mSystemsArray.empty())
			mSystemsArray.emplace_back(std::unique_ptr<SystemType>(new SystemType(mRegistry, arguments...)), 0);
		else
			mSystemsArray.emplace_back(std::unique_ptr<SystemType>(new SystemType(mRegistry, arguments...)), mSystemsArray.back().order + 1);
	}

	template<typename SystemType, typename ...Args>
	void SystemsQueue::appendSystemWithLastOrder(Args ...arguments)
	{
		mSystemsArray.emplace_back(std::unique_ptr<SystemType>(new SystemType(mRegistry, arguments...)), mSystemsArray.back().order);
	}

	template<typename SystemType, typename ...Args>
	void SystemsQueue::insertSystem(size_t order, Args ...arguments)
	{
		auto lastWithOrder = mSystemsArray.cend();
		--lastWithOrder;
		while (lastWithOrder != mSystemsArray.begin())
		{
			if (lastWithOrder->order == order)
			{
				++lastWithOrder;
				break;
			}
			--lastWithOrder;
		}

		mSystemsArray.insert(lastWithOrder, { std::unique_ptr<SystemType>(new SystemType(mRegistry, arguments...)), order });
	}

}
