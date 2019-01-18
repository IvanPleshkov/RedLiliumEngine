#pragma once

#include <Core/Common.h>
#include <Reflection/Serialization.h>
#include "Component.h"

namespace RED_LILIUM_NAMESPACE
{

class Scene;

class Entity final : public Serializable
{
public:
	RED_LILIUM_CLASS(Entity, Serializable);

	Entity();
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
	ptr<Entity> GetChild(u64 index = 0);
	ptr<const Entity> GetChild(u64 index = 0) const;
	ptr<Entity> AddChild(u64 position = u64_max);

	template<class T> ptr<T> GetComponent(u64 index = 0);
	template<class T> ptr<const T> GetComponent(u64 index = 0) const;
	template<class T> ptr<T> AddComponent(u64 position = u64_max);

private:
	ptr<Scene> m_scene;
	ptr<Entity> m_parent;
	std::vector<uptr<Entity>> m_children;
	std::vector<uptr<Component>> m_components;
	mat4 m_transform;
	std::string m_name;
};

}  // namespace RED_LILIUM_NAMESPACE
