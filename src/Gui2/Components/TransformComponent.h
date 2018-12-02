#pragma once

#include "Gui2Forward.h"
#include "Component.h"

namespace RED_LILIUM_NAMESPACE
{

class TransformComponent final : public Component
{
public:
	TransformComponent(ptr<Entity> entity);
	~TransformComponent() override = default;

private:
	vec2 m_size;
	mat3 m_transform;
};

} // namespace RED_LILIUM_NAMESPACE
