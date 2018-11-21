#pragma once

#include "Gui2Forward.h"

namespace RED_LILIUM_NAMESPACE
{

class Entity final : public RedLiliumObject
{
public:
	~Entity() override;

	template<class TComponent>
	ptr<TComponent> GetComponent();

	template<class TComponent>
	ptr<TComponent> AddComponent();

private:
	std::vector<uptr<Component>> m_components;
};

} // namespace RED_LILIUM_NAMESPACE
