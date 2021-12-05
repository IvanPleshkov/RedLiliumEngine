#include "resources_manager.h"
#include "mesh.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

ResourcesManager::ResourcesManager()
{
    spdlog::info("Working path: " + std::filesystem::current_path().string());

	std::ifstream pathJsonFileStream("paths.json");
	std::string pathJsonData(
		(std::istreambuf_iterator<char>(pathJsonFileStream)),
		std::istreambuf_iterator<char>());

	if (!pathJsonData.empty())
	{
        nlohmann::json pathsJson = nlohmann::json::parse(pathJsonData);
		if (pathsJson.find("resourcesPath") != pathsJson.end())
		{
			_path = pathsJson["resourcesPath"].get<std::string>();
		}
		else
		{
			throw std::runtime_error("No resources path in paths.json");
		}
	}
	else
	{
        throw std::runtime_error("Not found paths.json");
	}
}

ResourcesManager::~ResourcesManager() { }

std::string ResourcesManager::readResourceData(std::string_view resource)
{
	std::stringstream ss;
	ss << _path << "/" << resource;
    std::ifstream fileStream(ss.str());
	if (fileStream.is_open())
	{
		std::string fileData(
			(std::istreambuf_iterator<char>(fileStream)),
			std::istreambuf_iterator<char>());
		return std::move(fileData);
	}
	else
	{
		return std::string();
	}
}

std::unique_ptr<Mesh> ResourcesManager::loadObj(std::string_view filename)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    std::stringstream ss;
    ss << _path << "/" << filename;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, ss.str().c_str()))
    {
        throw std::runtime_error("Cannot load obj file: " + err);
    }

    auto mesh = std::make_unique<Mesh>();
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            mesh->_vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            mesh->_vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            mesh->_vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            mesh->_vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
            mesh->_vertices.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);

            if (mesh->_indices.size() < std::numeric_limits<uint16_t>::max())
            {
                mesh->_indices.push_back(mesh->_indices.size());
            }
            else
            {
                throw std::runtime_error("uint16_t overflow while model loading");
            }
        }
    }
    return mesh;
}
