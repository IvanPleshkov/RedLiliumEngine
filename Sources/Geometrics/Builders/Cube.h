#pragma once

#include <Render/RenderCommon.h>
#include "TransformBase.h"

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Cube final : public TransformBase<Cube>
{
public:
	Cube(Context& context);

	void Prepare(Context& context);

private:

};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
