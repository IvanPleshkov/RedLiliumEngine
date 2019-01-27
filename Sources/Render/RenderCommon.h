#pragma once

#include <Core/Common.h>
#include <Core/Flags.h>
#include <Core/Mesh.h>
#include <Core/Texture.h>
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

	enum class VertexAttribute : u32
	{
		Position = 1 << 0,
		Normal = 1 << 1,
		Tangent = 1 << 2,
		Bitangent = 1 << 3,
		Color0 = 1 << 4,
		Color1 = 1 << 5,
		Color2 = 1 << 6,
		Color3 = 1 << 7,
		TexCoord0 = 1 << 10,
		TexCoord1 = 1 << 11,
		TexCoord2 = 1 << 12,
		TexCoord3 = 1 << 13,
		TexCoord4 = 1 << 14,
		TexCoord5 = 1 << 15,
		TexCoord6 = 1 << 16,
		TexCoord7 = 1 << 17,
	};
	using VertexAttributeFlags = Flags<VertexAttribute>;

	enum class VertexType : u32
	{
		Uint8 = 1 << 0,
		Uint10 = 1 << 1, // Todo: Check capability
		Int16 = 1 << 2,
		Half = 1 << 3, // Todo: Check capability
		Float = 1 << 4,
	};

} // namespace RED_LILIUM_NAMESPACE
