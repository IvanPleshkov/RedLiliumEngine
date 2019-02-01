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
	class MaterialManager;

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

	enum class UniformType : u8
	{
		Sampler1D,
		Sampler2D,
		Sampler3D,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat2,
		Mat3,
		Mat4,
	};

	enum class ShaderType : u8
	{
		Vertex = 1 << 0,
		Fragment = 1 << 1,
	};

	struct Uniform
	{
		std::string name;
		UniformType type;
		u64 location;
		// todo: use compact_vector
		std::vector<char> value;

		void Apply();
	};

	template<class T>
	void SetUniformValue(const T& value);

} // namespace RED_LILIUM_NAMESPACE
