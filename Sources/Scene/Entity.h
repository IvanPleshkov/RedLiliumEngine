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

	const std::string& GetName() const;
	void SetName(const std::string& name);
	ptr<Scene> GetScene();
	ptr<const Scene> GetScene() const;
	ptr<Entity> GetParent();
	ptr<const Entity> GetParent() const;
	u64 GetChildrenCount() const;
	ptr<Entity> GetChild(u64 index = 0);
	ptr<const Entity> GetChild(u64 index = 0) const;
	ptr<Entity> GetChild(const std::string& name, u64 index = 0);
	ptr<const Entity> GetChild(const std::string& name, u64 index = 0) const;
	ptr<Entity> AddChild(const std::string& name, u64 position = u64_max);

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

template<class T>
inline ptr<T> Entity::GetComponent(u64 index)
{
	for (auto& component : m_components)
	{
		ptr<T> castedComponent = Cast<T>(component.get());
		if (castedComponent != nullptr)
		{
			if (index == 0)
			{
				return castedComponent;
			}
			else
			{
				index--;
			}
		}
	}
	return nullptr;
}

template<class T>
inline ptr<const T> Entity::GetComponent(u64 index) const
{
	for (const auto& component : m_components)
	{
		ptr<const T> castedComponent = Cast<const T>(component.get());
		if (castedComponent != nullptr)
		{
			if (index == 0)
			{
				return castedComponent;
			}
			else
			{
				index--;
			}
		}
	}
	return nullptr;
}

template<class T>
inline ptr<T> Entity::AddComponent(u64 position)
{
	uptr<T> component = umake<T>();
	ptr<T> result = component.get();
	if (position == u64_max)
	{
		m_components.push_back(std::move(component));
	}
	else
	{
		RED_LILIUM_ASSERT(position <= m_components.size());
		m_components.insert(m_components.begin() + position, std::move(component));
	}
	return result;
}

} // namespace RED_LILIUM_NAMESPACE
