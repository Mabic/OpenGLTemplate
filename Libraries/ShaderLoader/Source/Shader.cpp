#include "Shader.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	GLuint vertexShader = CompileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_COMPILE_STATUS, &isLinked);

	if (isLinked == GL_FALSE)
	{
		GLint logSize = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> errorLog(logSize + 1);
		glGetProgramInfoLog(m_program, logSize, &logSize, &errorLog[0]);

		std::copy(errorLog.begin(), errorLog.end(), std::ostream_iterator<GLchar>(std::cout));
	}

	glDetachShader(m_program, vertexShader);
	glDetachShader(m_program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string Shader::ReadShaderSource(const std::string& shaderPath) const
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		// Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();

		// close file handlers
		shaderFile.close();

		// Convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return shaderCode;
}

GLuint Shader::CompileShader(const std::string & vertexShaderPath, GLenum shaderType) const
{
	GLuint shader = glCreateShader(shaderType);
	const std::string shaderSource = ReadShaderSource(vertexShaderPath);
	const GLchar* vertexShaderSource = shaderSource.c_str();

	glShaderSource(shader, 1, &vertexShaderSource, NULL);

	glCompileShader(shader);

	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		std::vector<GLchar> errorLog(logSize + 1);
		glGetShaderInfoLog(shader, logSize, &logSize, &errorLog[0]);
		
		std::copy(errorLog.begin(), errorLog.end(), std::ostream_iterator<GLchar>(std::cout));
	}

	return shader;
}
