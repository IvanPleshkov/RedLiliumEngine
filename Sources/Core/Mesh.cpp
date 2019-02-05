#include "pch.h"
#include "Mesh.h"

using namespace RED_LILIUM_NAMESPACE;

uptr<Mesh> Mesh::GenerateCube()
{
	return nullptr;
}

uptr<Mesh> Mesh::GenerateTriangle()
{
	uptr<Mesh> mesh = umake<Mesh>();

	mesh->m_indices.push_back(0);
	mesh->m_indices.push_back(1);
	mesh->m_indices.push_back(2);

	mesh->m_positions.push_back({ -0.5f, -0.5f, 0.0f });
	mesh->m_positions.push_back({ 0.5f, -0.5f, 0.0f });
	mesh->m_positions.push_back({ 0.0f,  0.5f, 0.0f });

	mesh->m_colors0.push_back({ 1.0f, 0.0f, 0.0f, 1.0f });
	mesh->m_colors0.push_back({ 0.0f, 1.0f, 0.0f, 1.0f });
	mesh->m_colors0.push_back({ 0.0f, 0.0f, 1.0f, 1.0f });

	mesh->m_texCoords0.push_back({ -1.0f, -1.0f });
	mesh->m_texCoords0.push_back({ 1.0f, -1.0f });
	mesh->m_texCoords0.push_back({ 0.0f,  1.0f });

	return std::move(mesh);
}

uptr<Mesh> Mesh::GenerateRectangle()
{
	uptr<Mesh> mesh = umake<Mesh>();

	mesh->m_indices.push_back(0);
	mesh->m_indices.push_back(1);
	mesh->m_indices.push_back(3);
	mesh->m_indices.push_back(1);
	mesh->m_indices.push_back(2);
	mesh->m_indices.push_back(3);

	mesh->m_positions.push_back({ 1.0f,  1.0f, 0.0f});
	mesh->m_positions.push_back({ 1.0f, -1.0f, 0.0f});
	mesh->m_positions.push_back({-1.0f, -1.0f, 0.0f});
	mesh->m_positions.push_back({-1.0f,  1.0f, 0.0f });

	mesh->m_colors0.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	mesh->m_colors0.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	mesh->m_colors0.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });
	mesh->m_colors0.push_back({ 1.0f, 1.0f, 1.0f, 1.0f });

	mesh->m_texCoords0.push_back({ 1.0f, 1.0f });
	mesh->m_texCoords0.push_back({ 1.0f, 0.0f });
	mesh->m_texCoords0.push_back({ 0.0f, 0.0f });
	mesh->m_texCoords0.push_back({ 0.0f, 1.0f });

	return std::move(mesh);
}
