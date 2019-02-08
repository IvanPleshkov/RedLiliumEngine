#pragma once

#include <Core/Common.h>
#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class CameraComponent : public Component
{
public:
	RED_LILIUM_CLASS(MeshFilter, Component);

	CameraComponent();
	CameraComponent(ptr<Entity> parent);
	~CameraComponent() override = default;

private:
};

} // namespace RED_LILIUM_NAMESPACE
