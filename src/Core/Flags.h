#pragma once

#include "StdInclude.h"

namespace RED_LILIUM_NAMESPACE
{

template<typename T, typename FlagsSupportsOnlyEnumTypes = void>
class Flags;

template<typename T>
class Flags<T, typename std::enable_if_t<std::is_enum_v<T>>>
{
    using UnderlyingType = std::underlying_type_t<T>;
public:
	Flags() : m_value(static_cast<UnderlyingType>(0))
	{}

	explicit Flags(UnderlyingType value) : m_value(value)
	{}

    explicit Flags(T flag) : m_value(static_cast<UnderlyingType>(flag))
    {}

	bool Test(T flag) const
	{
		return (m_value & static_cast<UnderlyingType>(flag)) != 0;
	}

	Flags<T, void>& Add(T flag)
	{
		m_value |= static_cast<UnderlyingType>(flag);
        return *this;
	}

	void Clear()
	{
		m_value = 0;
	}

private:
    UnderlyingType m_value;
};

} // RED_LILIUM_NAMESPACE
