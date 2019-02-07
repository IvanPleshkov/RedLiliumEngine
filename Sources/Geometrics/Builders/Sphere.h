#pragma once

#include <Render/RenderCommon.h>
#include "TransformBase.h"

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Sphere final : public TransformBase<Sphere>
{
public:
	Sphere(Context& context);

	void Prepare(Context& context);

private:

};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
