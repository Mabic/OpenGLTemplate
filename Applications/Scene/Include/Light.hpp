#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm\glm.hpp>
#include "Mesh.hpp"

struct LightData {

	LightData() = default;

	LightData(glm::vec4 position, glm::vec4 color) 
		: m_position(position),
		  m_color(color)
	{}

	glm::vec4 m_position;
	glm::vec4 m_color;
};

class Light
{
public:

	Light(const LightData& data);
	Light(const LightData& data, const Mesh& mesh);
	Light(const Light&) = delete;
	Light(Light&&);
	~Light();

	void Render();
	inline const LightData& GetData() const { return m_data; }

private:
	void InitializeBuffers(const Mesh&);
	void CleanUp();

	LightData m_data;

	unsigned int m_vertexArrayObject;
	unsigned int m_vertexBufferObject;
	unsigned int m_elementBufferObject;
	unsigned int m_indicesSize;
};

#endif // !LIGHT_HPP