#include "pch.h"
#include "CameraComponent.h"

using namespace RED_LILIUM_NAMESPACE;

CameraComponent::CameraComponent()
	: Component()
{}

CameraComponent::CameraComponent(ptr<Entity> parent)
	: Component(parent)
{}

void CameraComponent::Serialize(ptr<Serializator> serializator) const
{}

void CameraComponent::Deserialize(ptr<const Serializator> serializator)
{}

const Camera& CameraComponent::GetCamera() const
{
	return m_camera;
}

void CameraComponent::SetCamera(const Camera& camera)
{
	m_camera = camera;
}
