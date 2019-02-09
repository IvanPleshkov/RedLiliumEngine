#pragma once

#include "../PipelineCommon.h"
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
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

private:
};

} // namespace RED_LILIUM_NAMESPACE
