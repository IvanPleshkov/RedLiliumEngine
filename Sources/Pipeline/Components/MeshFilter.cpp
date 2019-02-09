#include "pch.h"
#include "MeshFilter.h"

using namespace RED_LILIUM_NAMESPACE;

MeshFilter::MeshFilter()
	: Component()
{}

MeshFilter::MeshFilter(ptr<Entity> parent)
	: Component(parent)
{}

void MeshFilter::Serialize(ptr<Serializator> serializator) const
{}

void MeshFilter::Deserialize(ptr<const Serializator> serializator)
{}

const sptr<GpuMesh>& MeshFilter::GetGpuMesh() const
{
	return m_gpuMesh;
}

void MeshFilter::SetGpuMesh(const sptr<GpuMesh>& gpuMesh)
{
	m_gpuMesh = gpuMesh;
}

const sptr<Mesh>& MeshFilter::GetMesh() const
{
	return m_mesh;
}

void MeshFilter::SetMesh(const sptr<Mesh>& mesh)
{
	m_mesh = mesh;
}
