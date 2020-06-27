#include "pch.hpp"
#include "simRegion.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "ECS/entityUtil.hpp"
#include "debugVisualization.hpp"

namespace ph::system {

using namespace component;

static constexpr float partitionSideSize = 16.f * 12.f * 3.f;
static constexpr float simRegionSideSize = partitionSideSize * 3.f;
static const Vec2 partitionSize = Vec2(partitionSideSize);
static const Vec2i partitionSizeInt = Cast<Vec2i>(partitionSize);

void SimRegion::update(float dt)
{	
	PH_PROFILE_FUNCTION();

	Vec2 simRegionTriggerPos = isPlayerAlive() ? getPlayerCenterPos() : getCurrentCameraBounds().center();

	auto reComputeBounds = [&]()
	{
		mSimRegionCentralPartitionBounds.pos = Cast<Vec2>(hadamardMul(hadamardDiv(Cast<Vec2i>(simRegionTriggerPos), partitionSizeInt), partitionSizeInt));
		if(simRegionTriggerPos.x < 0.f) mSimRegionCentralPartitionBounds.x -= partitionSideSize;
		if(simRegionTriggerPos.y < 0.f) mSimRegionCentralPartitionBounds.y -= partitionSideSize;

		mSimRegionBounds.pos = mSimRegionCentralPartitionBounds.pos - partitionSize;
	};

	auto pullEntitiesInAndOutInsideRegion = [&]()
	{
		mRegistry.view<BodyRect>(entt::exclude<DontCareAboutSimRegion>).each([&]
		(auto entity, auto body)
		{
			if(intersect(mSimRegionBounds, body))
				mRegistry.assign_or_replace<InsideSimRegion>(entity);
			else
				mRegistry.reset<InsideSimRegion>(entity);
		});
	};

	if(!mInitialized)
	{
		mInitialized = true;

		DebugVisualization::setSimRegionBoundsPtr(&mSimRegionBounds);
		DebugVisualization::setSimRegionCentralPartitionBoundsPtr(&mSimRegionCentralPartitionBounds);

		mSimRegionCentralPartitionBounds.size = partitionSize;
		mSimRegionBounds.size = Vec2(simRegionSideSize);

		reComputeBounds();
		pullEntitiesInAndOutInsideRegion();

		return;
	}

	if(!mSimRegionCentralPartitionBounds.contains(simRegionTriggerPos))
	{
		reComputeBounds();
		pullEntitiesInAndOutInsideRegion();
	}
}

}

