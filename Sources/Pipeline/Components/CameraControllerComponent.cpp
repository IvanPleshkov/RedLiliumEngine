#include "pch.h"
#include "CameraControllerComponent.h"
#include "CameraComponent.h"
#include <Scene/SceneView.h>

using namespace RED_LILIUM_NAMESPACE;

CameraControllerComponent::CameraControllerComponent()
	: Component()
	, m_cameraController(nullptr)
{}

CameraControllerComponent::CameraControllerComponent(ptr<Entity> parent)
	: Component(parent)
	, m_cameraController(nullptr)
{}

void CameraControllerComponent::Serialize(ptr<Serializator> serializator) const
{}

void CameraControllerComponent::Deserialize(ptr<const Serializator> serializator)
{}

bool RED_LILIUM_NAMESPACE::HandleEvent(const MouseEvent& mouseEvent, span<ptr<const Entity>> entities)
{
	for (auto rendererEntity : SceneView<CameraControllerComponent, CameraComponent>(entities))
	{
		auto cameraControllerComponent = rendererEntity->GetComponent<const CameraControllerComponent>();
		auto cameraComponent = rendererEntity->GetComponent<const CameraComponent>();

	}
	return true;
}
