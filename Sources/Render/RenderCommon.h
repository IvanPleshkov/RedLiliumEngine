#pragma once

#include <Core/Common.h>
#include <Core/Flags.h>
#include <Core/Mesh.h>
#include <GL/glew.h>

namespace RED_LILIUM_NAMESPACE
{

	class RenderDevice;
	class RenderContext;
	class GpuResource;
	class GpuBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class GpuMesh;
	class GpuTexture;
	class Shader;
	class ShaderProgram;
	class Material;
	class VertexDeclaration;
	class VertexArrayObject;
	class MaterialManager;
	class GpuTextureManager;
	class UniformBlock;
	class RenderTarget;

	enum class VertexAttribute : u8
	{
		Position,
		Normal,
		Tangent,
		Bitangent,
		Color0,
		Color1,
		Color2,
		Color3,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		TexCoord4,
		TexCoord5,
		TexCoord6,
		TexCoord7,
	};
	struct VertexInput
	{
		VertexAttribute vertexAttribute;
		u8 layout;
	};

	enum class ShaderType : u8
	{
		Vertex = 1 << 0,
		Fragment = 1 << 1,
	};

	enum class RenderTargetComponent : u8
	{
		Color24 = 1 << 0,
		Depth24 = 1 << 1,
		Depth32 = 1 << 2,
		Stencil8 = 1 << 3,
		NeedSampleDepth = 1 << 4,
	};
	using RenderTargetFlags = Flags<RenderTargetComponent>;

} // namespace RED_LILIUM_NAMESPACE
