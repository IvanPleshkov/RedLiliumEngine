#pragma once

#include "RenderForward.h"

namespace RED_LILIUM_NAMESPACE
{

class IndexBuffer : public RedLiliumObject
{
public:
	IndexBuffer();
	~IndexBuffer() override = default;

	void Update(std::vector<u32>& indicies);

private:
};

} // namespace RED_LILIUM_NAMESPACE
