#pragma once

#include <Core/Common.h>
#include <Core/Reflection/Serialization.h>

namespace RED_LILIUM_NAMESPACE
{

class Scene;

class Entity : public Serializable
{
public:
	RED_LILIUM_CLASS(Entity, Serializable);

	Entity(ptr<Entity> parent);
	Entity(ptr<Scene> scene);
	~Entity() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	ptr<Scene> GetScene();
	ptr<const Scene> GetScene() const;
	ptr<Entity> GetParent();
	ptr<const Entity> GetParent() const;
	u64 GetChildrenCount() const;
	ptr<Entity> GetChild(u64 index);
	ptr<const Entity> GetChild(u64 index) const;
	template<class T> ptr<T> GetChild(u64 index = 0);
	template<class T> ptr<const T> GetChild(u64 index = 0) const;
	template<class T> ptr<T> AddChild();

private:
	ptr<Scene> m_scene;
	ptr<Entity> m_parent;
	std::vector<uptr<Entity>> m_children;
	mat4 m_transform;
	std::string m_name;
};

template<class T>
ptr<T> Entity::GetChild(u64 index)
{
	RED_LILIUM_ASSERT(false && "Not implemented");
	return nullptr;
}

template<class T>
ptr<const T> Entity::GetChild(u64 index) const
{
	RED_LILIUM_ASSERT(false && "Not implemented");
	return nullptr;
}

template<class T>
ptr<T> Entity::AddChild()
{
	uptr<T> child = umake<T>(this);
	ptr<T> result = child.get();
	m_children.push_back(std::move(child));
	return result;
}

}  // namespace RED_LILIUM_NAMESPACE
