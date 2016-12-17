#include "TransformationMaterialUBO.hpp"

#include <gl/glew.h>

#include "Mesh.hpp"
#include "Shader.h"

TransformationMaterialBuffer::TransformationMaterialBuffer(const Shader* shader)
{
	Initialize(shader);
}

TransformationMaterialBuffer::TransformationMaterialBuffer(TransformationMaterialBuffer&& buffer)
{
	m_uniformBufferObject = buffer.m_uniformBufferObject;

	buffer.m_uniformBufferObject = 0;
}

TransformationMaterialBuffer::~TransformationMaterialBuffer()
{
	CleanUp();
}

void TransformationMaterialBuffer::Initialize(const Shader* shader)
{
	unsigned int materialIndex = glGetUniformBlockIndex(shader->GetProgram(), "Material");
	glUniformBlockBinding(shader->GetProgram(), materialIndex, 0);

	int blockSize{ 0 };
	glGetActiveUniformBlockiv(shader->GetProgram(), materialIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	glGenBuffers(1, &m_uniformBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, materialIndex, m_uniformBufferObject);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void TransformationMaterialBuffer::CleanUp()
{
	glDeleteBuffers(1, &m_uniformBufferObject);
}

void TransformationMaterialBuffer::UpdateMaterial(const Material& material)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);

	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 0, sizeof(glm::vec4), &material.m_ambient);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 1, sizeof(glm::vec4), &material.m_diffuse);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 2, sizeof(glm::vec4), &material.m_specular);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 3, sizeof(float), &material.m_shinines);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}