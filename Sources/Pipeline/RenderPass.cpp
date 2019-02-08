#include "pch.h"
#include "RenderPass.h"
#include "RenderPipeline.h"

using namespace RED_LILIUM_NAMESPACE;

RenderPass::RenderPass(ptr<RenderPipeline> pipeline)
	: RedLiliumObject()
	, m_pipeline(pipeline)
{}
