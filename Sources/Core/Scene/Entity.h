#pragma once

#include "Common.h"
#include "Flags.h"
#include "Restorable.h"

namespace RED_LILIUM_NAMESPACE
{

class Entity : public Restorable
{
public:
	Entity();
	~Entity() override = default;

	ptr<Entity> GetParent();
	u64 GetChildrenCount() const;
	ptr<Entity> GetChild(u64 index);
	ptr<const Entity> GetChild(u64 index) const;
	template<class T> ptr<T> GetChild(u64 index);
	template<class T> ptr<const T> GetChild(u64 index) const;

private:
	ptr<Entity> m_parent;
	std::vector<uptr<Entity>> m_children;
};

}  // namespace RED_LILIUM_NAMESPACE
