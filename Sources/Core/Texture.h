#pragma once

#include "Common.h"

namespace RED_LILIUM_NAMESPACE
{

class TextureMip
{
public:

private:

};

class TextureBase : public RedLiliumObject
{
public:
	std::pair<u16, u16> GetSize() const;
	u8 GetChannelsCount() const;
	u8 GetMipsCount() const;

	Color32& GetRgba32(u16 width, u16 height);
	const Color32& GetRgba32(u16 width, u16 height) const;

	vec4& GetFloatPixel(u16 width, u16 height);
	const vec4& GetFloatPixel(u16 width, u16 height) const;

private:
	std::vector<std::vector<char>> m_data;
};

template<class T, u8 channels>
class Texture : public TextureBase
{
public:
	~Texture() override = default;

private:
	std::array<std::vector<T>, channels> m_data;
};

using TextureRgba32 = Texture<u8, 4>;
using TextureRgb32 = Texture<u8, 3>;
using TextureRgbaFloat = Texture<float, 4>;
using TextureRgbFloat = Texture<float, 3>;

} // namespace RED_LILIUM_NAMESPACE
