#include "TransformationMaterialUBO.hpp"

#include <gl/glew.h>

#include "Mesh.hpp"
#include "Light.hpp"
#include "Shader.h"

TransformationMaterialBuffer::TransformationMaterialBuffer()
	: m_uniformBufferObject(0)
{
	Initialize();
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

void TransformationMaterialBuffer::Initialize()
{
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_uniformBufferAlignSize);

	glGenBuffers(1, &m_uniformBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, m_uniformBufferAlignSize + sizeof(LightData), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uniformBufferObject, 0, sizeof(Material));
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_uniformBufferObject, m_uniformBufferAlignSize, sizeof(LightData));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void TransformationMaterialBuffer::CleanUp()
{
	glDeleteBuffers(1, &m_uniformBufferObject);
}

void TransformationMaterialBuffer::Bind(const Shader* shader)
{
	unsigned int materialIndex = glGetUniformBlockIndex(shader->GetProgram(), "Material");
	glUniformBlockBinding(shader->GetProgram(), materialIndex, 0);

	unsigned int lightIndex = glGetUniformBlockIndex(shader->GetProgram(), "Light");
	glUniformBlockBinding(shader->GetProgram(), lightIndex, 1);
}

void TransformationMaterialBuffer::UpdateMaterial(const Material& material)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Material), &material);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void TransformationMaterialBuffer::UpdateLight(const LightData& light)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);

	glBufferSubData(GL_UNIFORM_BUFFER, m_uniformBufferAlignSize, sizeof(LightData), &light);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}