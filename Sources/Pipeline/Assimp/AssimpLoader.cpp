#include "pch.h"
#include "AssimpLoader.h"
#include "AssimpMaterial.h"
#include <Render/RenderDevice.h>
#include <Render/GpuMesh.h>
#include <Render/Material.h>
#include <Core/FileSystem.h>
#include <Core/Camera.h>
#include <Scene/Component.h>
#include <Scene/Entity.h>
#include <Scene/Scene.h>
#include "Components/CameraComponent.h"
#include "Components/MeshFilter.h"
#include "Components/MeshRenderer.h"

using namespace RED_LILIUM_NAMESPACE;

namespace
{
struct LoadedData
{
	ptr<RenderDevice> m_renderDevice;
	AssimpImportOptions m_importOptions;

	std::vector<sptr<GpuMesh>> m_gpuMeshes;
	std::vector<sptr<Material>> m_materials;
	std::vector<sptr<Mesh>> m_cpuMesh;
	std::vector<std::pair<Camera, std::string>> m_cameras;
};

mat4 ConvertMatrix(const aiMatrix4x4 &aiMat)
{
	return {
	aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
	aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
	aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
	aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
	};
}

void loadData(const aiScene *scene, ptr<LoadedData> loadedData)
{
	// scene->mNumCameras
	for (u32 i = 0; i < scene->mNumCameras; i++)
	{
		aiCamera* aiCam = scene->mCameras[i];
		Camera cam;
		cam.SetPerspective(aiCam->mHorizontalFOV, aiCam->mAspect, aiCam->mClipPlaneNear, aiCam->mClipPlaneFar);
		auto aiPos = aiCam->mPosition;
		auto aiUp = aiCam->mUp;
		auto lookAt = aiCam->mLookAt;
		if (lookAt != aiPos)
		{
			cam.LookAt({ lookAt.x, lookAt.y, lookAt.z }, { aiPos.x, aiPos.y, aiPos.z }, { aiUp.x, aiUp.y, aiUp.z });
		}
		std::string aiName(aiCam->mName.C_Str());
		loadedData->m_cameras.push_back({cam, aiName});
	}

	// scene->mNumTextures;
	for (u32 i = 0; i < scene->mNumTextures; i++)
	{
		// todo (assimp)
		aiTexture* aitexture = scene->mTextures[i];
		// aitexture->
	}

	// scene->mNumAnimations;
	for (u32 i = 0; i < scene->mNumAnimations; i++)
	{
		// todo (assimp)
	}

	// scene->mNumLights;
	for (u32 i = 0; i < scene->mNumLights; i++)
	{
		// todo (assimp)

	}

	// scene->mNumMaterials;
	std::vector<sptr<Material>> uniqueMaterials;
	uniqueMaterials.reserve(scene->mNumMaterials);
	for (u32 i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* aiMat = scene->mMaterials[i];
		sptr<Material> material = loadedData->m_importOptions.materialFabric("");

		ptr<IAssimpMaterial> assimpMaterial = Cast<IAssimpMaterial>(material.get());
		if (assimpMaterial)
		{
		}

		uniqueMaterials.push_back(material);
	}

	// scene->mNumMeshes;
	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* aimesh = scene->mMeshes[i];
		std::string meshName = std::string(aimesh->mName.C_Str());
		if (meshName.empty())
		{
			meshName = "Mesh";
		}

		sptr<Mesh> mesh = smake<Mesh>();
		u32 vertsCount = aimesh->mNumVertices;

		for (u32 j = 0; j < aimesh->mNumFaces; j++)
		{
			aiFace face = aimesh->mFaces[j];
			for (u32 k = 0; k < face.mNumIndices; k++)
			{
				mesh->m_indices.push_back(face.mIndices[k]);
			}
		}

		if (aimesh->HasPositions())
		{
			mesh->m_positions.reserve(vertsCount);
			for (u32 j = 0; j < vertsCount; j++)
			{
				auto aipos = aimesh->mVertices[j];
				mesh->m_positions.push_back({ aipos.x, aipos.y, aipos.z });
			}
		}

		if (aimesh->HasNormals())
		{
			mesh->m_normals.reserve(vertsCount);
			for (u32 j = 0; j < vertsCount; j++)
			{
				auto ainormal = aimesh->mNormals[j];
				mesh->m_normals.push_back({ ainormal.x, ainormal.y, ainormal.z });
			}
		}

		if (aimesh->HasTangentsAndBitangents())
		{
			mesh->m_tangents.reserve(vertsCount);
			mesh->m_bitangents.reserve(vertsCount);
			for (u32 j = 0; j < vertsCount; j++)
			{
				auto aitangent = aimesh->mTangents[j];
				mesh->m_tangents.push_back({ aitangent.x, aitangent.y, aitangent.z });
				auto aibitangent = aimesh->mBitangents[j];
				mesh->m_bitangents.push_back({ aibitangent.x, aibitangent.y, aibitangent.z });
			}
		}

		for (u32 k = 0; k < std::min(Mesh::TEXCOORDS_COUNT, aimesh->GetNumUVChannels()); k++)
		{
			if (aimesh->HasTextureCoords(k))
			{
				mesh->m_texCoords[k].reserve(vertsCount);
				for (u32 j = 0; j < vertsCount; j++)
				{
					auto aiuv = aimesh->mTextureCoords[k][j];
					mesh->m_texCoords[k].push_back({ aiuv.x, aiuv.y });
				}
			}
		}

		for (u32 k = 0; k < std::min(Mesh::COLORS_COUNT, aimesh->GetNumColorChannels()); k++)
		{
			if (aimesh->HasVertexColors(k))
			{
				mesh->m_colors[k].reserve(vertsCount);
				for (u32 j = 0; j < vertsCount; j++)
				{
					auto aicolor = aimesh->mColors[k][j];
					mesh->m_colors[k].push_back({ aicolor.r, aicolor.g, aicolor.b, aicolor.a });
				}
			}
		}

		if (aimesh->HasBones())
		{
			// todo (assimp)
		}

		if (loadedData->m_importOptions.generateGpuMeshes && loadedData->m_renderDevice)
		{
			sptr<GpuMesh> gpuMesh = smake<GpuMesh>(loadedData->m_renderDevice, meshName);
			gpuMesh->Update(mesh.get());
			loadedData->m_gpuMeshes.push_back(std::move(gpuMesh));
		}

		loadedData->m_cpuMesh.push_back(std::move(mesh));
		if (aimesh->mMaterialIndex > 0 && aimesh->mMaterialIndex < uniqueMaterials.size())
		{
			loadedData->m_materials.push_back(uniqueMaterials[aimesh->mMaterialIndex]);
		}
		else
		{
			loadedData->m_materials.push_back(nullptr);
		}
	}
}

void processNode(aiNode *node, const aiScene *scene, ptr<Entity> resultEntity, ptr<LoadedData> loadedData)
{
	ptr<Entity> entity = resultEntity->AddChild(node->mName.C_Str());
	mat4 localTransform = ConvertMatrix(node->mTransformation);

	entity->SetLocalTransform(localTransform);

	for (u32 i = 0; i < node->mNumMeshes; i++)
	{
		ptr<Entity> meshEntity = entity->AddChild(loadedData->m_gpuMeshes[node->mMeshes[i]]->GetName());
		ptr<MeshFilter> filter = meshEntity->AddComponent<MeshFilter>();
		ptr<MeshRenderer> renderer = meshEntity->AddComponent<MeshRenderer>();

		filter->SetGpuMesh(loadedData->m_gpuMeshes[node->mMeshes[i]]);
		filter->SetMesh(loadedData->m_cpuMesh[node->mMeshes[i]]);

		sptr<Material> material = loadedData->m_materials[node->mMeshes[i]];
		renderer->SetMaterial(material);
	}

	if (loadedData->m_importOptions.loadCameras)
	{
		for (u32 i = 0; i < loadedData->m_cameras.size(); i++)
		{
			if (loadedData->m_cameras[i].second == entity->GetName())
			{
				continue;
			}

			Camera camera;
			camera.LookAt({ 5.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
			camera.SetPerspective(45.0f, 1.0f, 0.1f, 100.0f);

			ptr<CameraComponent> cameraComponent = entity->AddComponent<CameraComponent>();
			cameraComponent->SetCamera(camera);
		}
	}

	for (u32 i = 0; i < node->mNumChildren; i++)
	{
		aiNode* child = node->mChildren[i];
		processNode(child, scene, entity, loadedData);
	}
}
}

bool RED_LILIUM_NAMESPACE::LoadSceneByAssimp(ptr<RenderDevice> renderDevice, std::string_view filename, ptr<Entity> rootEntity, const AssimpImportOptions& options)
{
	std::string absoluteFilename = renderDevice->GetFileSystem()->ToAbsolute(filename);

	unsigned int flags = 0;
	flags |= aiProcess_Triangulate;
	if (options.splitMeshes)
	{
		flags |= aiProcess_SplitLargeMeshes;
	}
	if (options.generateNormals)
	{
		flags |= aiProcess_GenNormals;
	}
	if (options.optimizeMeshes)
	{
		flags |= aiProcess_OptimizeMeshes;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(absoluteFilename, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string message = "Failed to load model " + absoluteFilename;
		RED_LILIUM_LOG_ERROR(message);
		return false;
	}

	LoadedData loadedData;
	loadedData.m_renderDevice = renderDevice;
	loadedData.m_importOptions = options;
	loadData(scene, &loadedData);
	processNode(scene->mRootNode, scene, rootEntity, &loadedData);

	return true;
}
