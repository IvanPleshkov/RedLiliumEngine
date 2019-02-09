#include "pch.h"
#include "CameraControllerComponent.h"

using namespace RED_LILIUM_NAMESPACE;

CameraControllerComponent::CameraControllerComponent()
	: Component()
{}

CameraControllerComponent::CameraControllerComponent(ptr<Entity> parent)
	: Component(parent)
{}

void CameraControllerComponent::Serialize(ptr<Serializator> serializator) const
{}

void CameraControllerComponent::Deserialize(ptr<const Serializator> serializator)
{}
