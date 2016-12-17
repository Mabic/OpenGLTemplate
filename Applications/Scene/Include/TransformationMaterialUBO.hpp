#ifndef TRANSFORMATION_MATERIAL_BUFFER
#define TRANSFORMATION_MATERIAL_BUFFER

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
private:
	unsigned int m_uniformBufferObject;
};

#endif /* TRANSFORMATION_MATERIAL_BUFFER */