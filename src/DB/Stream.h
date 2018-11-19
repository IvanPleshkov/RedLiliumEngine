#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{
namespace db
{

struct IStream
{
	virtual std::vector<char> Read(u64 position, u64 size) = 0;
	virtual void Append(const std::vector<char>& data) = 0;
	virtual u64 Size() const = 0;
};


} // namespace db
} // namespace RED_LILIUM_NAMESPACE
