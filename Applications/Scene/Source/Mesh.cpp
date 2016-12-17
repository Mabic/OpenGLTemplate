#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<Texture>&& textures, 
	       std::vector<unsigned int>&& indices, Material&& material)
    : m_vertices(std::move(vertices)),
      m_textures(std::move(textures)),
      m_indices(std::move(indices)),
	  m_material(std::move(material))
{
}

Mesh::Mesh(Mesh&& mesh)
{
    this->m_vertices = std::move(mesh.m_vertices);
    this->m_textures = std::move(mesh.m_textures);
    this->m_indices = std::move(mesh.m_indices);
	this->m_material = std::move(mesh.m_material);
}