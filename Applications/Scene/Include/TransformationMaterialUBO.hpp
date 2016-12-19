#ifndef TRANSFORMATION_MATERIAL_BUFFER
#define TRANSFORMATION_MATERIAL_BUFFER

#include <glm\glm.hpp>

struct Material;
struct LightData;
class Shader;

class TransformationMaterialBuffer {
public:
	TransformationMaterialBuffer();
	TransformationMaterialBuffer(const TransformationMaterialBuffer&) = delete;
	TransformationMaterialBuffer(TransformationMaterialBuffer&&);
    ~TransformationMaterialBuffer();

	void Bind(const Shader*);
	void UpdateMaterial(const Material&);
	void UpdateLight(const LightData&);
private:
	void Initialize();
	void CleanUp();

	unsigned int m_uniformBufferObject;
	int m_uniformBufferAlignSize;
};

#endif /* TRANSFORMATION_MATERIAL_BUFFER */