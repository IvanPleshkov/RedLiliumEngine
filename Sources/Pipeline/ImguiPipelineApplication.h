#pragma once

#include "PipelineCommon.h"
#include <imgui/application.h>

namespace RED_LILIUM_NAMESPACE
{

class ImguiPipelineApplication : public ImguiApplication
{
public:
	ImguiPipelineApplication(ptr<RenderDevice> renderDevice);
	~ImguiPipelineApplication() override = default;

	void Init() override {}
	void Tick(f32 dTime) override {}
	void PollEvent(ptr<SDL_Event> event) override {}

private:
	uptr<RenderPipeline> m_pipeline;
};

} // namespace RED_LILIUM_NAMESPACE
