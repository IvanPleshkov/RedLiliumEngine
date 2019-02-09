#pragma once

#include "../PipelineCommon.h"
#include <Core/Camera.h>
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
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	const Camera& GetCamera() const;
	void SetCamera(const Camera& camera);

private:
	Camera m_camera;
};

} // namespace RED_LILIUM_NAMESPACE
