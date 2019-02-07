#pragma once

#include <Render/RenderCommon.h>
#include "BuilderBase.h"

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Bullet final : public BuilderBase<Bullet>
{
public:
	Bullet(Context& context);

	void Prepare(Context& context);

private:
};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
