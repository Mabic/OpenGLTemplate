#define GLEW_STATIC
#include <GL/glew.h>

#include "Mesh.hpp"
#include "Object.hpp"
#include "Shader.h"

Object::Object(const Mesh& mesh)
	: m_isTextureExist(!mesh.GetTextures().empty()),
	  m_isMeshRenderable(true)
{
	InitializeBuffers(mesh);
}

Object::Object(Object&& object)
{
	m_vertexArrayObject = object.m_vertexArrayObject;
	m_vertexBufferObject = object.m_vertexBufferObject;
	m_elementBufferObject = object.m_elementBufferObject;
	m_textureBufferObject = object.m_textureBufferObject;
	m_indicesSize = object.m_indicesSize;
	m_isTextureExist = object.m_isTextureExist;
	m_isMeshRenderable = object.m_isMeshRenderable;

	object.m_vertexArrayObject = 0;
	object.m_vertexBufferObject = 0;
	object.m_elementBufferObject = 0;
	object.m_textureBufferObject = 0;
	object.m_indicesSize = 0;
	object.m_isTextureExist = 0;
	object.m_isMeshRenderable = 0;
}

Object::~Object()
{
	CleanUp();
}

void Object::Render(const Shader& shader)
{
	glBindVertexArray(m_vertexArrayObject);

	// textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureBufferObject);
	glUniform1i(glGetUniformLocation(shader.GetProgram(), "textureSampler"), 0);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indicesSize), GL_UNSIGNED_INT, 0);
}

void Object::InitializeBuffers(const Mesh& mesh)
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

	// Set current VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture
	if (m_isTextureExist) {
		glGenTextures(1, &m_textureBufferObject);
		glBindTexture(GL_TEXTURE_2D, m_textureBufferObject);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mesh.GetTextures().at(0).GetWidth(), mesh.GetTextures().at(0).GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, mesh.GetTextures().at(0).GetTextureData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)(sizeof(GL_FLOAT) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Object::CleanUp()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteBuffers(1, &m_elementBufferObject);
	if (m_isTextureExist) {
		glDeleteTextures(1, &m_textureBufferObject);
	}
}