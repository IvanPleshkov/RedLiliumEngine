#pragma once

#include <Render/RenderCommon.h>

namespace RED_LILIUM_NAMESPACE
{

namespace Geometrics
{

class Context : public RedLiliumObject
{
public:
	Context();
	~Context() override;

	friend class ScopedState;
private:
};

} // namespace Geometrics

} // namespace RED_LILIUM_NAMESPACE
