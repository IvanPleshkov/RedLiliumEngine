#pragma once

#include "../PipelineCommon.h"
#include <Scene/Component.h>

namespace RED_LILIUM_NAMESPACE
{

class MeshFilter : public Component
{
public:
	RED_LILIUM_CLASS(MeshFilter, Component);

	MeshFilter();
	MeshFilter(ptr<Entity> parent);
	~MeshFilter() override = default;
	void Serialize(ptr<Serializator> serializator) const override;
	void Deserialize(ptr<const Serializator> serializator) override;

	const sptr<GpuMesh>& GetGpuMesh() const;
	void SetGpuMesh(const sptr<GpuMesh>& gpuMesh);

	const sptr<Mesh>& GetMesh() const;
	void SetMesh(const sptr<Mesh>& mesh);

private:
	sptr<GpuMesh> m_gpuMesh;
	sptr<Mesh> m_mesh;
};

} // namespace RED_LILIUM_NAMESPACE
