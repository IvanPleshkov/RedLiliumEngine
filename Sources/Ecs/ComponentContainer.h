#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

using ComponentTypeId = u32;

inline ComponentTypeId GenerateComponentTypeId()
{
	static ComponentTypeId id = 0;
	return id++;
}

template<class T>
inline ComponentTypeId GetComponentTypeId()
{
	static const ComponentTypeId id = GenerateComponentTypeId();
	return id;
}

class ComponentContainerBase
{
public:
	virtual ~ComponentContainerBase() = default;
	virtual uptr<ComponentContainerBase> CreateWithSameType() const = 0;
	virtual ComponentTypeId GetComponentTypeId() const = 0;
	virtual size_t Size() const = 0;
	virtual void SwapComponents(u32 index1, u32 index2) = 0;
	virtual void MoveComponent(ptr<ComponentContainerBase> other) = 0;
	virtual void PopComponent() = 0;
};

template<class TComponent>
class ComponentContainer : public ComponentContainerBase
{
public:
	~ComponentContainer() override = default;
	uptr<ComponentContainerBase> CreateWithSameType() const override
	{
		return umake<ComponentContainer<TComponent>>();
	}

	ComponentTypeId GetComponentTypeId() const override
	{
		return GetComponentTypeId<TComponent>();
	}

	size_t Size() const override
	{
		return m_components.size();
	}

	void SwapComponents(u32 index1, u32 index2) override
	{
		std::swap(m_components[index1], m_components[index2]);
	}

	void PushComponent(TComponent&& component) override
	{
		m_components.push_back(std::move(component));
	}

	void MoveComponent(ptr<ComponentContainerBase> other) override
	{
		ptr<ComponentContainer<TComponent>> casted = static_cast<ptr<ComponentContainer<TComponent>>>(other);
		RED_LILIUM_ASSERT(!casted->m_components.empty());
		m_components.push_back(std::move(casted->m_components[casted->m_components.size() - 1]));
		casted->m_components.pop_back();
	}

	void PopComponent() override
	{
		m_components.pop_back();
	}

	std::vector<TComponent> m_components;
};

} // namespace RED_LILIUM_NAMESPACE
