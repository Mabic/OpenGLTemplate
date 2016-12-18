#ifndef TRANSFORMATION_MATERIAL_BUFFER
#define TRANSFORMATION_MATERIAL_BUFFER

#include <glm\glm.hpp>

struct Material;
class Shader;

class TransformationMaterialBuffer {
public:
	TransformationMaterialBuffer() = delete;
	TransformationMaterialBuffer(const Shader*);
	TransformationMaterialBuffer(const TransformationMaterialBuffer&) = delete;
	TransformationMaterialBuffer(TransformationMaterialBuffer&&);
    ~TransformationMaterialBuffer();

	void Initialize(const Shader*);
	void CleanUp();

	void UpdateMaterial(const Material&);
	void UpdateLight(glm::vec4 position, glm::vec3 color);
private:
	unsigned int m_uniformBufferObject;
};

#endif /* TRANSFORMATION_MATERIAL_BUFFER */