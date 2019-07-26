#include <catch.hpp>

#include <AI/aStar.hpp>

namespace ph {

TEST_CASE("Node operator < works correctly")
{
	Node node({1, 0}, {0, 0}, 1, 2);
	
	Node nodeWithBiggerTotalCost({1, 0}, {0, 0}, 2, 2);
	CHECK(node < nodeWithBiggerTotalCost);

	Node nodeWithTheSameTotalCostButWithBiggerRealDistanceFromStartNode({1, 0}, {0, 0}, 2, 1);
	CHECK(node < nodeWithTheSameTotalCostButWithBiggerRealDistanceFromStartNode);
}

}