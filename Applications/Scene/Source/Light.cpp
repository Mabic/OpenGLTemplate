#include <GL/glew.h>

#include "Light.hpp"

Light::Light(const LightData& data)
	: m_data(data),
	  m_vertexArrayObject(0),
	  m_vertexBufferObject(0),
	  m_elementBufferObject(0),
	  m_indicesSize(0)  
{

}

Light::Light(const LightData& data, const Mesh& mesh)
	: m_data(data)
{
	InitializeBuffers(mesh);
}

Light::Light(Light&& light)
{
	m_data = light.m_data;
	m_vertexArrayObject = light.m_vertexArrayObject;
	m_vertexBufferObject = light.m_vertexBufferObject;
	m_elementBufferObject = light.m_elementBufferObject;
	m_indicesSize = light.m_indicesSize;

	light.m_data = LightData();
	light.m_vertexArrayObject = 0;
	light.m_vertexBufferObject = 0;
	light.m_elementBufferObject = 0;
	light.m_indicesSize = 0;
}

Light::~Light()
{
	CleanUp();
}

void Light::UpdateLightData(LightData&& lightData)
{
	m_data = lightData;
}

void Light::Render()
{
	if (m_indicesSize == 0) {
		return;
	}

	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indicesSize), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Light::InitializeBuffers(const Mesh& mesh)
{
	m_indicesSize = mesh.GetIndices().size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, mesh.GetVertices().size() * sizeof(Vertex), &mesh.GetVertices().front(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndices().size() * sizeof(unsigned int), &mesh.GetIndices().front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Light::CleanUp()
{
	glDeleteBuffers(1, &m_elementBufferObject);
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}