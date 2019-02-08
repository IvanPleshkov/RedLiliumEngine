#include "pch.h"
#include "OpaquePass.h"
#include "../RenderPipeline.h"

using namespace RED_LILIUM_NAMESPACE;

OpaquePass::OpaquePass(ptr<RenderPipeline> pipeline)
	: RenderPass(pipeline)
{}

bool OpaquePass::Render()
{
	return true;
}
