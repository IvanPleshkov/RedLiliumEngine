#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

struct Rectangle
{

};

using Shape = std::variant<>;

struct FillByColor
{
	Color32 Color;
};

struct FillByTexture
{
	vec2 UvMin;
	vec2 UvMax;
};

using FillMode = std::variant<FillByColor, FillByTexture>;

class GeometryNode
{
	FillMode m_fillMode;
	mat3 m_localTRansform;
};

} // namespace RED_LILIUM_NAMESPACE
