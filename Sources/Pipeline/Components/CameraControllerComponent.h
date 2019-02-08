#pragma once

#include <Core/Common.h>
#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class CameraControllerComponent : public Component
{
public:
	RED_LILIUM_CLASS(MeshFilter, Component);

	CameraControllerComponent();
	CameraControllerComponent(ptr<Entity> parent);
	~CameraControllerComponent() override = default;

private:
};

} // namespace RED_LILIUM_NAMESPACE
