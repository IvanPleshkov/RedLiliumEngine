#pragma once

#include "RenderCommon.h"

namespace RED_LILIUM_NAMESPACE
{

class Material : public RedLiliumObject
{
public:
	Material();
	~Material() override;
	ptr<VertexDeclaration> GetVertexDeclaration();
	void SetVertexDeclaration(ptr<VertexDeclaration> vertexDeclaration);
	const sptr<ShaderProgram>& GetShaderProgram();
	void SetShaderProgram(const sptr<ShaderProgram>& shaderProgram);

private:
	ptr<VertexDeclaration> m_vertexDeclaration;
	sptr<ShaderProgram> m_shaderProgram;
};

} // namespace RED_LILIUM_NAMESPACE
