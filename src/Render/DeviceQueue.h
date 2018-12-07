#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{
namespace Render
{

class DeviceQueue : public RedLiliumObject
{
public:
	~DeviceQueue() override;

RED_LILIUM_INTERNAL:
	DeviceQueue();

private:
};

} // namespace Render
} // namespace RED_LILIUM_NAMESPACE
