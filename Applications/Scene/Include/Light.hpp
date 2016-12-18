#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm\glm.hpp>
#include "Mesh.hpp"

class Light
{
public:

	Light(glm::vec4 position, glm::vec3 color);
	Light(glm::vec4 position, glm::vec3 color, const Mesh& mesh);
	Light(const Light&) = delete;
	Light(Light&&);
	~Light();

	void Render();
	inline glm::vec3 GetPosition() const { return m_position; }

private:
	void InitializeBuffers(const Mesh&);
	void CleanUp();

	glm::vec4 m_position;
	glm::vec3 m_color;

	unsigned int m_vertexArrayObject;
	unsigned int m_vertexBufferObject;
	unsigned int m_elementBufferObject;
	unsigned int m_indicesSize;
};

#endif // !LIGHT_HPP