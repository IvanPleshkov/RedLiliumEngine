#include "pch.h"
#include "MeshRenderer.h"
#include <Render/Material.h>

using namespace RED_LILIUM_NAMESPACE;

MeshRenderer::MeshRenderer()
	: Component()
{}

MeshRenderer::MeshRenderer(ptr<Entity> parent)
	: Component(parent)
{}

void MeshRenderer::Serialize(ptr<Serializator> serializator) const
{}

void MeshRenderer::Deserialize(ptr<const Serializator> serializator)
{}

const sptr<Material>& MeshRenderer::GetMaterial() const
{
	return m_material;
}

void MeshRenderer::SetMaterial(const sptr<Material>& material)
{
	m_material = material;
}
