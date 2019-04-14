#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

using ChunkIndex = u16;
const ChunkIndex nullChunkIndex = u16_max;

using EntityGeneration = u16;
const EntityGeneration maxEntityGeneration = u16_max;

using EntityIndex = u32;

struct Entity final
{
	u64 Hash() const;
	bool operator==(const Entity& other) const;
	bool operator!=(const Entity& other) const;
	bool operator<(const Entity& other) const;

	EntityIndex m_index = u32_max;
	EntityGeneration m_generation = u16_max;
	ChunkIndex m_chunk = nullChunkIndex;
};

} // namespace RED_LILIUM_NAMESPACE

namespace std
{
	template <>
	struct hash<RED_LILIUM_NAMESPACE::Entity>
	{
		std::size_t operator()(const RED_LILIUM_NAMESPACE::Entity& k) const
		{
			return hash<RED_LILIUM_NAMESPACE::u64>()(k.Hash());
		}
	};
}
