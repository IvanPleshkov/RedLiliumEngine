#pragma once

#include <Core/Common.h>
#include <Render/RenderCommon.h>

namespace RED_LILIUM_NAMESPACE
{

class Entity;
class AssimpMaterial;

struct AssimpImportOptions
{
	bool optimizeMeshes = false;
	bool splitMeshes = false;
	bool generateNormals = true;
	bool saveCpuMeshes = true;
	bool generateGpuMeshes = true;

	bool loadCameras = false;
	bool loadLights = true;

	sptr<AssimpMaterial> material;
};

bool LoadSceneByAssimp(ptr<RenderDevice> renderDevice, std::string_view filename, ptr<Entity> rootEntity, const AssimpImportOptions& options);

} // namespace RED_LILIUM_NAMESPACE
