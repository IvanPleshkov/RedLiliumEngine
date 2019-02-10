#include "pch.h"
#include "CameraControllerComponent.h"
#include "CameraComponent.h"

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

void CameraControllerComponent::Update()
{}

bool CameraControllerComponent::HandleMouseEvent(const MouseEvent& mouseEvent, const InputState& inputState)
{
	if (!m_cameraController)
	{
		m_cameraController = GetEntity()->GetComponent<CameraComponent>();
		if (!m_cameraController)
		{
			return false;
		}
	}

	return true;
}
