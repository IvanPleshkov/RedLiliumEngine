#pragma once

#include "RenderCommon.h"
#include "GpuBuffer.h"

namespace RED_LILIUM_NAMESPACE
{

class RenderTarget : public GpuResource
{
public:
	RenderTarget(ptr<RenderDevice> renderDevice);
	~RenderTarget() override;

	void Bind();
	void Unbind();
	void Create(
		u32 width,
		u32 height,
		RenderTargetFlags flags = RenderTargetFlags());

	const sptr<GpuTexture>& GetDepth() const;
	const sptr<GpuTexture>& GetStencil() const;
	const sptr<GpuTexture>& GetColor() const;

private:
	u32 m_width;
	u32 m_height;
	sptr<GpuTexture> m_depth;
	sptr<GpuTexture> m_stencil;
	sptr<GpuTexture> m_color;
	GLuint m_renderBuffer;
};

} // namespace RED_LILIUM_NAMESPACE
