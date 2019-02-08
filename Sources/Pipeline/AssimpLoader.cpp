#include "pch.h"
#include "AssimpLoader.h"

using namespace RED_LILIUM_NAMESPACE;

bool RED_LILIUM_NAMESPACE::LoadSceneByAssimp(ptr<FileSystem> filesystem, std::string_view filename, ptr<Entity> rootEntity)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(std::string(filename), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
	{
		return false;
	}

	return true;
}
