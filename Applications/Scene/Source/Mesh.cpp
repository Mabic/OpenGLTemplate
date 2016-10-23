#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex>&& vertices,std::vector<Texture>&& textures, std::vector<unsigned int>&& indices)
	: m_vertices(std::move(vertices)),
	  m_textures(std::move(textures)),
	  m_indices(std::move(indices))
{
}

Mesh::Mesh(Mesh&& mesh)
{
	this->m_vertices = std::move(mesh.m_vertices);
	this->m_textures = std::move(mesh.m_textures);
	this->m_indices = std::move(mesh.m_indices);
}