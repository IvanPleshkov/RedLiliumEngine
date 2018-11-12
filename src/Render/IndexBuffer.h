#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{

class IndexBuffer : public RedLiliumObject
{
public:
	~IndexBuffer(sptr<Device> device) override = default;

	void Update(std::vector<u32>& indicies);

private:
	sptr<Device> m_device;
};

} // namespace RED_LILIUM_NAMESPACE
