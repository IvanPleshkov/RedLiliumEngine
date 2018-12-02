#pragma once

#include "Gui2Forward.h"

namespace RED_LILIUM_NAMESPACE
{

class Entity final : public RedLiliumObject
{
public:
	Entity(ptr<Component> owner, const std::vector<StyleId>& styles);
	~Entity() override = default;

	template<class TComponent>
	ptr<TComponent> GetComponent();

	template<class TComponent>
	ptr<TComponent> AddComponent();

	void DumpToJson(json& j);

private:
	ptr<Component> m_owner;
	std::vector<StyleId> m_styles;
	std::unordered_map<std::type_index, uptr<Component>> m_components;
};

template<class TComponent>
ptr<TComponent> Entity::GetComponent()
{
	const auto typeIndex = std::type_index(typeid(TComponent));
	if (m_components[typeIndex] == m_components.end())
	{
		return nullptr;
	}

	ptr<Component> component = m_components[typeIndex].get();
	ptr<TComponent> castedComponent = Cast<TComponent>(component);
	RED_LILIUM_ASSERT(castedComponent != nullptr);
	return castedComponent;
}

template<class TComponent>
ptr<TComponent> Entity::AddComponent()
{
	const auto typeIndex = std::type_index(typeid(TComponent));
	RED_LILIUM_ASSERT(m_components.find(typeIndex) == m_components.end());
	m_components.insert({ typeIndex, umake<TComponent>(this) });
	return GetComponent<TComponent>();
}

} // namespace RED_LILIUM_NAMESPACE
