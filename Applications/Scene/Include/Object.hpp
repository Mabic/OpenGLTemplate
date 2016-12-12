#ifndef OBJECT_HPP
#define OBJECT_HPP

class Mesh;
class Shader;

class Object
{
public:
	Object(const Mesh&);
	Object(const Object&) = delete;
	Object(Object&&);
	~Object();

	void Render(const Shader&);

private:

	void InitializeBuffers(const Mesh&);
	void CleanUp();

	unsigned int m_vertexArrayObject;
	unsigned int m_vertexBufferObject;
	unsigned int m_elementBufferObject;
	unsigned int m_textureBufferObject;

	size_t m_indicesSize;
	bool m_isTextureExist;
};

#endif // !OBJECT_HPP