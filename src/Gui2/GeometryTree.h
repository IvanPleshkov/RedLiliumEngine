#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

struct FillByColor
{
	Color32 Color;
};

struct FillByTexture
{
	vec2 Min;
	vec2 Max;
};

using FillMode = std::variant<FillByColor, FillByTexture>;

struct GeometryNode
{
	FillMode FillMode;

};

} // namespace RED_LILIUM_NAMESPACE
