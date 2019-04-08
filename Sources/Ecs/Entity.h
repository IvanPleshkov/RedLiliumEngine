#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class Entity final
{
public:
	bool IsValid()
	{
		return !IsTemporary() && m_index != u32_max;
	}

	bool IsTemporary()
	{
		return m_cluster == u16_max;
	}

	friend class Scene;
	friend class Transaction;
private:

	u32 m_index = u32_max;
	u16 m_generation = u16_max;
	u16 m_cluster = u16_max;
};

} // namespace RED_LILIUM_NAMESPACE
