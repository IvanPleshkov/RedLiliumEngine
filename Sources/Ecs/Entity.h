#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

using ChunkIndex = u16;
const ChunkIndex nullChunkIndex = u16_max;

using EntityGeneration = u16;

using EntityIndex = u32;

struct Entity final
{
	bool IsValid()
	{
		return m_index != u32_max;
	}

	EntityIndex m_index = u32_max;
	EntityGeneration m_generation = u16_max;
	ChunkIndex m_chunk = nullChunkIndex;
};

} // namespace RED_LILIUM_NAMESPACE
