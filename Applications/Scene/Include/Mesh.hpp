#ifndef MESH_HPP
#define MESH_HPP

#include <glm\glm.hpp>
#include <vector>
#include <memory>

#include "Texture.hpp"

struct Vertex {

    Vertex(glm::vec3&& position, glm::vec3&& normal = glm::vec3(0.0f,0.0f,0.0f), 
           glm::vec2&& textureCoordinate = glm::vec2(0.0f, 0.0f)) 
        :  m_position(std::move(position)),
           m_normals(std::move(normal)),
           m_textureCoordinates(std::move(textureCoordinate))
    {
    }

    Vertex(glm::vec3&& position, glm::vec2&& textureCoordinate = glm::vec2(0.0f, 0.0f), 
           glm::vec3&& normal = glm::vec3(0.0f, 0.0f, 0.0f))
        :  m_position(std::move(position)),
           m_normals(std::move(normal)),
           m_textureCoordinates(std::move(textureCoordinate))
    {
    }

    glm::vec3 m_position;
    glm::vec3 m_normals;
    glm::vec2 m_textureCoordinates;
};

struct Material {

	Material()
		: m_ambient(glm::vec3(1.0f)),
		  m_diffuse(glm::vec3(1.0f)),
		  m_specular(glm::vec3(1.0f)),
		  m_shinines(1.0f)
	{
	}

	Material(glm::vec3&& ambient, glm::vec3 diffuse, glm::vec3 specular, float shinines)
		: m_ambient(std::move(ambient)),
		  m_diffuse(std::move(diffuse)),
		  m_specular(std::move(specular)),
		  m_shinines(shinines)
	{
	}

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	float m_shinines;
};

class Mesh {
public:
    Mesh() = delete;
    Mesh(const Mesh& mesh) = delete;
    Mesh(Mesh&& mesh);
    Mesh(std::vector<Vertex>&& vertices, std::vector<Texture>&& textures, 
		 std::vector<unsigned int>&& indices, Material&& material);
    ~Mesh() = default;

    inline const std::vector<Vertex>& GetVertices() const { return m_vertices; }
    inline const std::vector<Texture>& GetTextures() const { return m_textures; }
    inline const std::vector<unsigned int>& GetIndices() const { return m_indices; }
	inline const Material& GetMaterial() const { return m_material; }

private:
    std::vector<Vertex> m_vertices;
    std::vector<Texture> m_textures;
    std::vector<unsigned int> m_indices;
	Material m_material;
};

#endif // !MESH_HPP
