#pragma once

#include <Core/Common.h>
#include "../RenderPass.h"

namespace RED_LILIUM_NAMESPACE
{

class OpaquePass : public RenderPass
{
public:
	OpaquePass(ptr<RenderPipeline> pipeline);
	~OpaquePass() override = default;

	bool Render() override;

private:
};

} // namespace RED_LILIUM_NAMESPACE
