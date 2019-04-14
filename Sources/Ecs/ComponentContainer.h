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
};

template<class TComponent>
class ComponentContainer : public ComponentContainerBase
{
public:
	~ComponentContainer() override = default;

private:
	std::deque<TComponent> m_components;
};

} // namespace RED_LILIUM_NAMESPACE
