#pragma once

#include <Core/Common.h>
#include "DrawBuilders.h"

namespace RED_LILIUM_NAMESPACE
{

class DrawContext : public RedLiliumObject
{
public:
	void PushTransform(mat3 transform, vec4 scissor);
	void PopTransform();



private:
};

} // namespace RED_LILIUM_NAMESPACE
