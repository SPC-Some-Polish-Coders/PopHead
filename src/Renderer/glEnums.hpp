#pragma once

namespace ph {

enum class DataUsage
{
	streamDraw,
	staticDraw,
	dynamicDraw,
};

unsigned toGLEnum(DataUsage);

}
