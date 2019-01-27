#pragma once

#include "RenderCommon.h"
#include <Core/ApplicationSettings.h>

namespace RED_LILIUM_NAMESPACE
{

class RenderDevice : public RedLiliumObject
{
public:
	RenderDevice(ptr<ApplicationSettings> applicationSettings);
	~RenderDevice() override;

	uptr<RenderContext> CreateRenderContext();

	ptr<VertexDeclaration> GetVertexDeclarationP() { return m_declP.get(); }
	ptr<VertexDeclaration> GetVertexDeclarationPN() { return m_declPN.get(); }
	ptr<VertexDeclaration> GetVertexDeclarationPC() { return m_declPC.get(); }
	ptr<VertexDeclaration> GetVertexDeclarationPNC() { return m_declPNC.get(); }
	ptr<VertexDeclaration> GetVertexDeclarationPT() { return m_declPT.get(); }
	ptr<VertexDeclaration> GetVertexDeclarationPNT() { return m_declPNT.get(); }

	void CheckErrors();

private:
	void InitStandardVertexDeclarations();

	ptr<ApplicationSettings> m_applicationSettings;
	uptr<VertexDeclaration> m_declP;
	uptr<VertexDeclaration> m_declPN;
	uptr<VertexDeclaration> m_declPC;
	uptr<VertexDeclaration> m_declPNC;
	uptr<VertexDeclaration> m_declPT;
	uptr<VertexDeclaration> m_declPNT;
};

} // namespace RED_LILIUM_NAMESPACE
