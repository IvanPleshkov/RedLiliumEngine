#pragma once

#include <Core/Common.h>
#include <Core/Flags.h>

namespace RED_LILIUM_NAMESPACE
{

class Entity : public RedLiliumObject
{
public:
	Entity();
	~Entity() override = default;

private:
	std::vector<uptr<Entity>> m_children;
};

}  // namespace RED_LILIUM_NAMESPACE
