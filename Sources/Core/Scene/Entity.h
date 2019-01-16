#pragma once

#include "Common.h"
#include "Flags.h"
#include "Restorable.h"

namespace RED_LILIUM_NAMESPACE
{

class Scene;

class Entity : public Restorable
{
public:
	Entity(ptr<Entity> parent);
	Entity(ptr<Scene> scene);
	~Entity() override = default;

	ptr<Scene> GetScene();
	ptr<const Scene> GetScene() const;
	ptr<Entity> GetParent();
	ptr<const Entity> GetParent() const;
	u64 GetChildrenCount() const;
	ptr<Entity> GetChild(u64 index);
	ptr<const Entity> GetChild(u64 index) const;
	template<class T> ptr<T> GetChild(u64 index);
	template<class T> ptr<const T> GetChild(u64 index) const;

private:
	ptr<Scene> m_scene;
	ptr<Entity> m_parent;
	std::vector<uptr<Entity>> m_children;
	mat4 m_transform;
	std::string m_name;
};

}  // namespace RED_LILIUM_NAMESPACE
