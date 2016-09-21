#ifndef SHADER_H
#define SHADER_H
#include <string>

#include <GL/glew.h>

class Shader {
public:
	Shader() = delete;
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader() = default;

	inline void UseProgram() const { glUseProgram(m_program); }
private:
	std::string ReadShaderSource(const std::string& shaderPath) const;
	GLuint CompileShader(const std::string& vertexShaderPath, GLenum shaderType) const;
	GLuint m_program;
};

#endif /* SHADER_H */