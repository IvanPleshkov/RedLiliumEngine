#pragma once

#include "../PipelineCommon.h"
#include <Scene/Component.h>
#include <Core/InputEvent.h>

namespace RED_LILIUM_NAMESPACE
{

class CameraComponent;

class CameraControllerComponent : public Component
{
public:
	RED_LILIUM_CLASS(MeshFilter, Component);

	CameraControllerComponent();
	CameraControllerComponent(ptr<Entity> parent);
	~CameraControllerComponent() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
	ptr<CameraComponent> m_cameraController;
	vec3 m_up;
	vec3 m_lookAt;
};

bool HandleEvent(const MouseEvent& mouseEvent, span<ptr<const Entity>> entities);

} // namespace RED_LILIUM_NAMESPACE
