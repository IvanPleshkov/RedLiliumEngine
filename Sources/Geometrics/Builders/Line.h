#pragma once

#include <Render/RenderCommon.h>
#include "BuilderBase.h"

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Line final : public BuilderBase<Line>
{
public:
	Line(Context& context);

	void Prepare(Context& context);

private:

};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
