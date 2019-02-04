#include "pch.h"
#include "GpuTexture.h"

using namespace RED_LILIUM_NAMESPACE;

namespace
{
	struct GpuTextureSendData
	{
		GLenum format;
		GLenum type;
		GLsizei width;
		GLsizei height;
		std::vector<char> data;
	};

	template<
		size_t SrcChannelsCount,
		class SrcFormat,
		size_t DestChannelsCount,
		class DestRedFormat,
		class DestGreenFormat,
		class DestBlueFormat,
		class DestAlphaFormat>
	GpuTextureSendData ConvertOpencvMatTemplatedFinal(const cv::Mat& src, GLenum format, GLenum type)
	{
		struct Pixel
		{
			DestRedFormat r = 0;
			DestGreenFormat g = 0;
			DestBlueFormat b = 0;
			DestAlphaFormat a = 0;
		};

		size_t pixelSize = 0;
		switch (DestChannelsCount)
		{
		case 1:
			pixelSize = sizeof(DestRedFormat);
			break;
		case 2:
			pixelSize = sizeof(DestRedFormat) + sizeof(DestGreenFormat);
			break;
		case 3:
			pixelSize = sizeof(DestRedFormat) + sizeof(DestGreenFormat) + sizeof(DestBlueFormat);
			break;
		case 4:
			pixelSize = sizeof(DestRedFormat) + sizeof(DestGreenFormat) + sizeof(DestBlueFormat) + sizeof(DestAlphaFormat);
			break;
		default:
			RED_LILIUM_NOT_IMPLEMENTED();
		}

		GpuTextureSendData result;
		result.format = format;
		result.type = type;
		result.width = src.cols;
		result.height = src.rows;
		result.data.resize(pixelSize * src.cols * src.rows, char(0));

		Pixel pixel;
		u64 resultPos = 0;
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				auto srcPixel = ((const std::array<SrcFormat, SrcChannelsCount>*)(src.data + src.step.p[0] * i))[j];

				if (SrcChannelsCount > 0)
				{
					pixel.r = static_cast<DestRedFormat>(srcPixel[0]);
				}
				if (SrcChannelsCount > 1)
				{
					pixel.g = static_cast<DestGreenFormat>(srcPixel[1]);
				}
				if (SrcChannelsCount > 2)
				{
					pixel.b = static_cast<DestBlueFormat>(srcPixel[2]);
				}
				if (SrcChannelsCount > 3)
				{
					pixel.a = static_cast<DestBlueFormat>(srcPixel[3]);
				}


				std::memcpy(result.data.data() + resultPos, &pixel, pixelSize);
				resultPos += pixelSize;
			}
		}

		RED_LILIUM_ASSERT(resultPos == result.data.size());
		return std::move(result);
	}

	template<
		size_t DestChannelsCount,
		class DestRedFormat,
		class DestGreenFormat,
		class DestBlueFormat,
		class DestAlphaFormat>
	GpuTextureSendData ConvertOpencvMatTemplated(const cv::Mat& src, GLenum format, GLenum type)
	{
		switch (src.depth()) {
		case CV_8U:
			switch (src.channels())
			{
			case 1:
				return std::move(ConvertOpencvMatTemplatedFinal<
					1, u8, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 2:
				return std::move(ConvertOpencvMatTemplatedFinal<
					2, u8, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 3:
				return std::move(ConvertOpencvMatTemplatedFinal<
					3, u8, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 4:
				return std::move(ConvertOpencvMatTemplatedFinal<
					4, u8, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			default:
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			break;

		case CV_8S:
			RED_LILIUM_NOT_IMPLEMENTED();

		case CV_16U:
			switch (src.channels())
			{
			case 1:
				return std::move(ConvertOpencvMatTemplatedFinal<
					1, u16, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 2:
				return std::move(ConvertOpencvMatTemplatedFinal<
					2, u16, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 3:
				return std::move(ConvertOpencvMatTemplatedFinal<
					3, u16, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 4:
				return std::move(ConvertOpencvMatTemplatedFinal<
					4, u16, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			default:
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			break;

		case CV_16F:
			RED_LILIUM_NOT_IMPLEMENTED();

		case CV_16S:
			RED_LILIUM_NOT_IMPLEMENTED();

		case CV_32S:
			RED_LILIUM_NOT_IMPLEMENTED();

		case CV_32F:
			switch (src.channels())
			{
			case 1:
				return std::move(ConvertOpencvMatTemplatedFinal<
					1, f32, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 2:
				return std::move(ConvertOpencvMatTemplatedFinal<
					2, f32, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 3:
				return std::move(ConvertOpencvMatTemplatedFinal<
					3, f32, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 4:
				return std::move(ConvertOpencvMatTemplatedFinal<
					4, f32, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			default:
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			break;

		case CV_64F:
			switch (src.channels())
			{
			case 1:
				return std::move(ConvertOpencvMatTemplatedFinal<
					1, f64, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 2:
				return std::move(ConvertOpencvMatTemplatedFinal<
					2, f64, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 3:
				return std::move(ConvertOpencvMatTemplatedFinal<
					3, f64, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			case 4:
				return std::move(ConvertOpencvMatTemplatedFinal<
					4, f64, DestChannelsCount, DestRedFormat, DestGreenFormat, DestBlueFormat, DestAlphaFormat>(src, format, type));

			default:
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			break;

		default:
			RED_LILIUM_NOT_IMPLEMENTED();
		}
		return {};
	}

	GpuTextureSendData ConvertOpencvMat(const cv::Mat& src, GLenum format, GLenum type)
	{
		switch (format)
		{
		case GL_RED:
			switch (type)
			{
			case GL_UNSIGNED_BYTE:
				return ConvertOpencvMatTemplated<1, u8, u8, u8, u8>(src, format, type);

			case GL_UNSIGNED_SHORT:
				return ConvertOpencvMatTemplated<1, u16, u16, u16, u16>(src, format, type);

			case GL_FLOAT:
				return ConvertOpencvMatTemplated<1, f32, f32, f32, f32>(src, format, type);

			case GL_INT:
				return ConvertOpencvMatTemplated<1, i32, i32, i32, i32>(src, format, type);

			case GL_UNSIGNED_INT:
				return ConvertOpencvMatTemplated<1, u32, u32, u32, u32>(src, format, type);

			default:
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			break;

		case GL_RG:
			switch (type)
			{
			case GL_UNSIGNED_BYTE:
				return ConvertOpencvMatTemplated<2, u8, u8, u8, u8>(src, format, type);

			case GL_UNSIGNED_SHORT:
				return ConvertOpencvMatTemplated<2, u16, u16, u16, u16>(src, format, type);

			case GL_FLOAT:
				return ConvertOpencvMatTemplated<2, f32, f32, f32, f32>(src, format, type);

			case GL_INT:
				return ConvertOpencvMatTemplated<2, i32, i32, i32, i32>(src, format, type);

			case GL_UNSIGNED_INT:
				return ConvertOpencvMatTemplated<2, u32, u32, u32, u32>(src, format, type);

			default:
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			break;

		case GL_RGB:
			switch (type)
			{
			case GL_UNSIGNED_BYTE:
				return ConvertOpencvMatTemplated<3, u8, u8, u8, u8>(src, format, type);

			case GL_UNSIGNED_SHORT:
				return ConvertOpencvMatTemplated<3, u16, u16, u16, u16>(src, format, type);

			case GL_FLOAT:
				return ConvertOpencvMatTemplated<3, f32, f32, f32, f32>(src, format, type);

			case GL_INT:
				return ConvertOpencvMatTemplated<3, i32, i32, i32, i32>(src, format, type);

			case GL_UNSIGNED_INT:
				return ConvertOpencvMatTemplated<3, u32, u32, u32, u32>(src, format, type);

			default:
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			break;


		case GL_RGBA:
			switch (type)
			{
			case GL_UNSIGNED_BYTE:
				return ConvertOpencvMatTemplated<4, u8, u8, u8, u8>(src, format, type);

			case GL_UNSIGNED_SHORT:
				return ConvertOpencvMatTemplated<4, u16, u16, u16, u16>(src, format, type);

			case GL_FLOAT:
				return ConvertOpencvMatTemplated<4, f32, f32, f32, f32>(src, format, type);

			case GL_INT:
				return ConvertOpencvMatTemplated<4, i32, i32, i32, i32>(src, format, type);

			case GL_UNSIGNED_INT:
				return ConvertOpencvMatTemplated<4, u32, u32, u32, u32>(src, format, type);

			default:
				RED_LILIUM_NOT_IMPLEMENTED();
			}
			break;

		default:
			RED_LILIUM_NOT_IMPLEMENTED();
		}
		return {};
	}

	GpuTextureSendData ConvertOpencvMat(const cv::Mat& src, TextureFormat destFormat)
	{
		switch (destFormat)
		{
		case rl::TextureFormat::R8:
			return std::move(ConvertOpencvMat(src, GL_RED, GL_UNSIGNED_BYTE));

		case rl::TextureFormat::R8_SNORM:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R16:
			return std::move(ConvertOpencvMat(src, GL_RED, GL_UNSIGNED_SHORT));

		case rl::TextureFormat::R16_SNORM:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RG8:
			return std::move(ConvertOpencvMat(src, GL_RG, GL_UNSIGNED_BYTE));

		case rl::TextureFormat::RG8_SNORM:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RG16:
			return std::move(ConvertOpencvMat(src, GL_RG, GL_UNSIGNED_SHORT));

		case rl::TextureFormat::RG16_SNORM:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R3_G3_B2:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB4:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB5:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB8:
			return std::move(ConvertOpencvMat(src, GL_RGB, GL_UNSIGNED_BYTE));

		case rl::TextureFormat::RGB8_SNORM:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB10:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB12:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB16_SNORM:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA2:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA4:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB5_A1:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA8:
			return std::move(ConvertOpencvMat(src, GL_RGBA, GL_UNSIGNED_BYTE));

		case rl::TextureFormat::RGBA8_SNORM:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB10_A2:
			return std::move(ConvertOpencvMat(src, GL_RGBA, GL_UNSIGNED_INT_10_10_10_2));

		case rl::TextureFormat::RGB10_A2UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA12:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA16:
			return std::move(ConvertOpencvMat(src, GL_RGBA, GL_UNSIGNED_SHORT));

		case rl::TextureFormat::SRGB8:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::SRGB8_ALPHA8:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R16F:
			return std::move(ConvertOpencvMat(src, GL_R, GL_HALF_FLOAT));

		case rl::TextureFormat::RG16F:
			return std::move(ConvertOpencvMat(src, GL_RG, GL_HALF_FLOAT));

		case rl::TextureFormat::RGB16F:
			return std::move(ConvertOpencvMat(src, GL_RGB, GL_HALF_FLOAT));

		case rl::TextureFormat::RGBA16F:
			return std::move(ConvertOpencvMat(src, GL_RGBA, GL_HALF_FLOAT));

		case rl::TextureFormat::R32F:
			return std::move(ConvertOpencvMat(src, GL_R, GL_FLOAT));

		case rl::TextureFormat::RG32F:
			return std::move(ConvertOpencvMat(src, GL_RG, GL_FLOAT));

		case rl::TextureFormat::RGB32F:
			return std::move(ConvertOpencvMat(src, GL_RGB, GL_FLOAT));

		case rl::TextureFormat::RGBA32F:
			return std::move(ConvertOpencvMat(src, GL_RGBA, GL_FLOAT));

		case rl::TextureFormat::R11F_G11F_B10F:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB9_E5:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R8I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R8UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R16I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R16UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R32I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::R32UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RG8I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RG8UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RG16I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RG16UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RG32I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RG32UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB8I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB8UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB16I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB16UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB32I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGB32UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA8I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA8UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA16I:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA16UI:
			RED_LILIUM_NOT_IMPLEMENTED();

		case rl::TextureFormat::RGBA32I:
			return std::move(ConvertOpencvMat(src, GL_RGBA, GL_INT));

		case rl::TextureFormat::RGBA32UI:
			return std::move(ConvertOpencvMat(src, GL_RGBA, GL_UNSIGNED_INT));

		default:
			RED_LILIUM_NOT_IMPLEMENTED();
		}

		return {};
	}
}

GpuTexture::GpuTexture(ptr<RenderDevice> renderDevice, const TextureSettings& settings)
	: GpuResource(renderDevice)
{
	glGenTextures(1, &m_handler);
	Settings(settings);
}

GpuTexture::~GpuTexture()
{
	if (m_handler != 0)
	{
		glDeleteTextures(1, &m_handler);
	}
}

void GpuTexture::Settings(const TextureSettings& settings)
{
	m_settings = settings;
	switch (m_settings.dims)
	{
	case 1:
		m_glDimensions = GL_TEXTURE_1D;
		break;
	case 2:
		m_glDimensions = GL_TEXTURE_2D;
		break;
	case 3:
		m_glDimensions = GL_TEXTURE_3D;
		break;
	default:
		RED_LILIUM_ASSERT(false && "Wrong texture dimensions");
	}

	Bind();

	switch (m_settings.wrapping)
	{
	case TextureWrapping::Repeat:
		glTexParameteri(m_glDimensions, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_glDimensions, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case TextureWrapping::MirroredRepeat:
		glTexParameteri(m_glDimensions, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(m_glDimensions, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case TextureWrapping::ClampToEdge:
		glTexParameteri(m_glDimensions, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_glDimensions, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case TextureWrapping::ClampToBorder:
		glTexParameteri(m_glDimensions, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(m_glDimensions, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;
	default:
		RED_LILIUM_ASSERT(false && "Wrong texture wrapping");
		break;
	}

	// todo: use correct filter settings
	if (m_settings.needMips)
	{
		if (m_settings.filtering == TextureFiltering::Linear)
		{
			glTexParameteri(m_glDimensions, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(m_glDimensions, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(m_glDimensions, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(m_glDimensions, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
	else
	{
		if (m_settings.filtering == TextureFiltering::Linear)
		{
			glTexParameteri(m_glDimensions, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(m_glDimensions, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(m_glDimensions, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(m_glDimensions, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
	Unbind();
}

const TextureSettings& GpuTexture::Settings()
{
	return m_settings;
}

bool GpuTexture::SendData(const cv::Mat& mat)
{
	Bind();
	switch (m_settings.dims)
	{
	case 1:
		RED_LILIUM_ASSERT(false && "cv::Mat cannot have 1 dimension");
		Unbind();
		return false;
	case 2:
		{
			auto data = ConvertOpencvMat(mat, m_settings.format);
			auto glType = GetGlType(m_settings.format);
			glTexImage2D(
				m_glDimensions,
				0,
				GetGlType(m_settings.format),
				data.width,
				data.height,
				0,
				data.format,
				data.type,
				data.data.data());
		}
		break;
	case 3:
		RED_LILIUM_NOT_IMPLEMENTED();
		break;
	default:
		RED_LILIUM_ASSERT(false && "Wrong texture dimensions");
		Unbind();
		return false;
	}

	if (m_settings.needMips)
	{
		glGenerateMipmap(m_glDimensions);
	}
	Unbind();
	return true;
}

GLint GpuTexture::GetGlType(TextureFormat textureFormat)
{
	switch (textureFormat)
	{
	case TextureFormat::R8:
		return GL_R8;
	case TextureFormat::R8_SNORM:
		return GL_R8_SNORM;
	case TextureFormat::R16:
		return GL_R16;
	case TextureFormat::R16_SNORM:
		return GL_R16_SNORM;
	case TextureFormat::RG8:
		return GL_RG8;
	case TextureFormat::RG8_SNORM:
		return GL_RG8_SNORM;
	case TextureFormat::RG16:
		return GL_RG16;
	case TextureFormat::RG16_SNORM:
		return GL_RG16_SNORM;
	case TextureFormat::R3_G3_B2:
		return GL_R3_G3_B2;
	case TextureFormat::RGB4:
		return GL_RGB4;
	case TextureFormat::RGB5:
		return GL_RGB5;
	case TextureFormat::RGB8:
		return GL_RGB8;
	case TextureFormat::RGB8_SNORM:
		return GL_RGB8_SNORM;
	case TextureFormat::RGB10:
		return GL_RGB10;
	case TextureFormat::RGB12:
		return GL_RGB12;
	case TextureFormat::RGB16_SNORM:
		return GL_RGB16_SNORM;
	case TextureFormat::RGBA2:
		return GL_RGBA2;
	case TextureFormat::RGBA4:
		return GL_RGBA4;
	case TextureFormat::RGB5_A1:
		return GL_RGB5_A1;
	case TextureFormat::RGBA8:
		return GL_RGBA8;
	case TextureFormat::RGBA8_SNORM:
		return GL_RGBA8_SNORM;
	case TextureFormat::RGB10_A2:
		return GL_RGB10_A2;
	case TextureFormat::RGB10_A2UI:
		return GL_RGB10_A2UI;
	case TextureFormat::RGBA12:
		return GL_RGBA12;
	case TextureFormat::RGBA16:
		return GL_RGBA16;
	case TextureFormat::SRGB8:
		return GL_SRGB8;
	case TextureFormat::SRGB8_ALPHA8:
		return GL_SRGB8_ALPHA8;
	case TextureFormat::R16F:
		return GL_R16F;
	case TextureFormat::RG16F:
		return GL_RG16F;
	case TextureFormat::RGB16F:
		return GL_RGB16F;
	case TextureFormat::RGBA16F:
		return GL_RGBA16F;
	case TextureFormat::R32F:
		return GL_R32F;
	case TextureFormat::RG32F:
		return GL_RG32F;
	case TextureFormat::RGB32F:
		return GL_RGB32F;
	case TextureFormat::RGBA32F:
		return GL_RGBA32F;
	case TextureFormat::R11F_G11F_B10F:
		return GL_R11F_G11F_B10F;
	case TextureFormat::RGB9_E5:
		return GL_RGB9_E5;
	case TextureFormat::R8I:
		return GL_R8I;
	case TextureFormat::R8UI:
		return GL_R8UI;
	case TextureFormat::R16I:
		return GL_R16I;
	case TextureFormat::R16UI:
		return GL_R16UI;
	case TextureFormat::R32I:
		return GL_R32I;
	case TextureFormat::R32UI:
		return GL_R32UI;
	case TextureFormat::RG8I:
		return GL_RG8I;
	case TextureFormat::RG8UI:
		return GL_RG8UI;
	case TextureFormat::RG16I:
		return GL_RG16I;
	case TextureFormat::RG16UI:
		return GL_RG16UI;
	case TextureFormat::RG32I:
		return GL_RG32I;
	case TextureFormat::RG32UI:
		return GL_RG32UI;
	case TextureFormat::RGB8I:
		return GL_RGB8I;
	case TextureFormat::RGB8UI:
		return GL_RGB8UI;
	case TextureFormat::RGB16I:
		return GL_RGB16I;
	case TextureFormat::RGB16UI:
		return GL_RGB16UI;
	case TextureFormat::RGB32I:
		return GL_RGB32I;
	case TextureFormat::RGB32UI:
		return GL_RGB32UI;
	case TextureFormat::RGBA8I:
		return GL_RGBA8I;
	case TextureFormat::RGBA8UI:
		return GL_RGBA8UI;
	case TextureFormat::RGBA16I:
		return GL_RGBA16I;
	case TextureFormat::RGBA16UI:
		return GL_RGBA16UI;
	case TextureFormat::RGBA32I:
		return GL_RGBA32I;
	case TextureFormat::RGBA32UI:
		return GL_RGBA32UI;
	default:
		RED_LILIUM_ASSERT(false && "Unsupported texture format");
		return 0;
	}
}

void GpuTexture::Bind()
{
	glBindTexture(m_glDimensions, m_handler);
}

void GpuTexture::Unbind()
{
	glBindTexture(m_glDimensions, 0);
}
