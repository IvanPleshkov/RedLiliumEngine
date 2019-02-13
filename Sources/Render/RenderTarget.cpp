#include "pch.h"
#include "RenderTarget.h"
#include "GpuTexture.h"

using namespace RED_LILIUM_NAMESPACE;

RenderTarget::RenderTarget(ptr<RenderDevice> renderDevice, std::string_view resourceName)
	: GpuResource(renderDevice, resourceName)
	, m_width(0)
	, m_height(0)
	, m_depth(nullptr)
	, m_stencil(nullptr)
	, m_color(nullptr)
{
	glGenFramebuffers(1, &m_handler);
}

RenderTarget::~RenderTarget()
{
	if (m_handler != 0)
	{
		glDeleteFramebuffers(1, &m_handler);
		m_handler = 0;
	}
}

void RenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_handler);
}

void RenderTarget::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::Create(u32 width, u32 height, RenderTargetFlags flags)
{
	Bind();

	if (flags.Empty())
	{
		flags.Add(RenderTargetComponent::Color24);
		flags.Add(RenderTargetComponent::Depth24);
		flags.Add(RenderTargetComponent::Stencil8);
	}

	m_width = width;
	m_height = height;

	if (flags.Test(RenderTargetComponent::Color24))
	{
		TextureSettings colorSettings;
		colorSettings.type = TextureType::Texture2D;
		colorSettings.format = TextureFormat::RGB8;
		colorSettings.wrapping = TextureWrapping::Repeat;
		colorSettings.filtering = TextureFiltering::Linear;
		colorSettings.needMips = false;

		m_color = smake<GpuTexture>(m_renderDevice, colorSettings);
		m_color->Create(width, height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color->GetNative(), 0);
	}
	else
	{
		m_color = nullptr;
	}

	if (flags.Test(RenderTargetComponent::NeedSampleDepth))
	{
		if (flags.Test(RenderTargetComponent::Depth24) && flags.Test(RenderTargetComponent::Stencil8))
		{
			TextureSettings depthSettings;
			depthSettings.type = TextureType::Texture2D;
			depthSettings.format = TextureFormat::D24S8;
			depthSettings.wrapping = TextureWrapping::Repeat;
			depthSettings.filtering = TextureFiltering::Linear;
			depthSettings.needMips = false;

			m_depth = smake<GpuTexture>(m_renderDevice, depthSettings);
			m_depth->Create(width, height);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth->GetNative(), 0);
		}
		else
		{
			if (flags.Test(RenderTargetComponent::Depth24) && !flags.Test(RenderTargetComponent::Stencil8))
			{
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			if (!flags.Test(RenderTargetComponent::Depth24) && flags.Test(RenderTargetComponent::Stencil8))
			{
				RED_LILIUM_NOT_IMPLEMENTED();
			}
		}
	}
	else
	{
		if (flags.Test(RenderTargetComponent::Depth24) && flags.Test(RenderTargetComponent::Stencil8))
		{
			glGenRenderbuffers(1, &m_renderBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
		}
		else
		{
			if (flags.Test(RenderTargetComponent::Depth24) && !flags.Test(RenderTargetComponent::Stencil8))
			{
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			if (!flags.Test(RenderTargetComponent::Depth24) && flags.Test(RenderTargetComponent::Stencil8))
			{
				RED_LILIUM_NOT_IMPLEMENTED();
			}
		}
	}

	if (flags.Test(RenderTargetComponent::Depth32))
	{
		RED_LILIUM_NOT_IMPLEMENTED();
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		RED_LILIUM_ASSERT(false && "Wrong settings for Render Target");
	}

	Unbind();
}

const sptr<GpuTexture>& RenderTarget::GetDepth() const
{
	return m_depth;
}

const sptr<GpuTexture>& RenderTarget::GetStencil() const
{
	return m_stencil;
}

const sptr<GpuTexture>& RenderTarget::GetColor() const
{
	return m_color;
}
