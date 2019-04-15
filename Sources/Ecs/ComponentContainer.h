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
	virtual void SwapComponents(u32 index1, u32 index2) = 0;
	virtual void Invalidate(u32 index) = 0;
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

	void SwapComponents(u32 index1, u32 index2) override
	{
		std::swap(m_components[index1], m_components[index2]);
	}

	void Invalidate(u32 index) override
	{
		m_components[index] = TComponent();
	}

	std::vector<TComponent> m_components;
};

} // namespace RED_LILIUM_NAMESPACE
