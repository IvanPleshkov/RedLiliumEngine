#pragma once

#include "Render/RenderCommon.h"
#include "GpuBuffer.h"

namespace RED_LILIUM_NAMESPACE
{

	enum class TextureFormat : u8
	{
		R8,				// 	R		8
		R8_SNORM,		// 	R		s8
		R16,			// 	R		16
		R16_SNORM,		// 	R		s16
		RG8,			// 	RG		8 	8
		RG8_SNORM,		// 	RG		s8 	s8
		RG16,			// 	RG		16 	16
		RG16_SNORM,		// 	RG		s16 	s16
		R3_G3_B2,		// 	RGB		3 	3 	2
		RGB4,			// 	RGB		4 	4 	4
		RGB5,			// 	RGB		5 	5 	5
		RGB8,			// 	RGB		8 	8 	8
		RGB8_SNORM,		// 	RGB		s8 	s8 	s8
		RGB10,			// 	RGB		10 	10 	10
		RGB12,			// 	RGB		12 	12 	12
		RGB16_SNORM,	// 	RGB		16 	16 	16
		RGBA2,			// 	RGB		2 	2 	2 	2
		RGBA4,			// 	RGB		4 	4 	4 	4
		RGB5_A1,		// 	RGBA	5 	5 	5 	1
		RGBA8,			// 	RGBA	8 	8 	8 	8
		RGBA8_SNORM,	// 	RGBA	s8 	s8 	s8 	s8
		RGB10_A2,		// 	RGBA	10 	10 	10 	2
		RGB10_A2UI,		// 	RGBA	ui10 	ui10 	ui10 	ui2
		RGBA12,			// 	RGBA	12 	12 	12 	12
		RGBA16,			// 	RGBA	16 	16 	16 	16
		SRGB8,			// 	RGB		8 	8 	8
		SRGB8_ALPHA8,	// 	RGBA	8 	8 	8 	8
		R16F,			// 	R		f16
		RG16F,			// 	RG		f16 	f16
		RGB16F,			// 	RGB		f16 	f16 	f16
		RGBA16F,		// 	RGBA	f16 	f16 	f16 	f16
		R32F,			// 	R		f32
		RG32F,			// 	RG		f32 	f32
		RGB32F,			// 	RGB		f32 	f32 	f32
		RGBA32F,		// 	RGBA	f32 	f32 	f32 	f32
		R11F_G11F_B10F,	// 	RGB		f11 	f11 	f10
		RGB9_E5,		// 	RGB		9 	9 	9 	  	5
		R8I,			// 	R		i8
		R8UI,			// 	R		ui8
		R16I,			// 	R		i16
		R16UI,			// 	R		ui16
		R32I,			// 	R		i32
		R32UI,			// 	R		ui32
		RG8I,			// 	RG		i8 	i8
		RG8UI,			// 	RG		ui8 	ui8
		RG16I,			// 	RG		i16 	i16
		RG16UI,			// 	RG		ui16 	ui16
		RG32I,			// 	RG		i32 	i32
		RG32UI,			// 	RG		ui32 	ui32
		RGB8I,			// 	RGB		i8 	i8 	i8
		RGB8UI,			// 	RGB		ui8 	ui8 	ui8
		RGB16I,			// 	RGB		i16 	i16 	i16
		RGB16UI,		// 	RGB		ui16 	ui16 	ui16
		RGB32I,			// 	RGB		i32 	i32 	i32
		RGB32UI,		// 	RGB		ui32 	ui32 	ui32
		RGBA8I,			// 	RGBA	i8 	i8 	i8 	i8
		RGBA8UI,		// 	RGBA	ui8 	ui8 	ui8 	ui8
		RGBA16I,		// 	RGBA	i16 	i16 	i16 	i16
		RGBA16UI,		// 	RGBA	ui16 	ui16 	ui16 	ui16
		RGBA32I,		// 	RGBA	i32 	i32 	i32 	i32
		RGBA32UI,		// 	RGBA	ui32 	ui32 	ui32 	ui32

		D24S8,			// DS		24		8
		D32,			// D		32
	};

enum class TextureWrapping : u8
{
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder
};

enum class TextureFiltering : u8
{
	Nearest,
	Linear
};

enum class TextureType : u8
{
	Texture1D,
	Texture2D,
	Texture3D,
	TextureCube
};

struct TextureSettings
{
	TextureType type				= TextureType::Texture2D;
	TextureFormat format			= TextureFormat::RGBA8;
	TextureWrapping wrapping		= TextureWrapping::Repeat;
	TextureFiltering filtering		= TextureFiltering::Linear;
	bool needMips					= true;
};

class GpuTexture : public GpuResource
{
public:
	GpuTexture(ptr<RenderDevice> renderDevice, const TextureSettings& settings, std::string_view resourceName = "");
	~GpuTexture();

	void Settings(const TextureSettings& settings);
	const TextureSettings& Settings();
	bool SendData(const cv::Mat& mat);
	bool Create(u32 width, u32 height = 0, u32 depth = 0);

	void Bind();
	void Unbind();

private:
	static GLint GetGlType(TextureFormat textureFormat);

	GLenum m_glDimensions;
	TextureSettings m_settings;
};

} // namespace RED_LILIUM_NAMESPACE
