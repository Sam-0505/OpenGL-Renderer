#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

// Constructor
ShaderProgram::ShaderProgram()
	:mHandle(0)
{
}

// Destructor
ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(mHandle);
}

//-----------------------------------------------------------------------------
// Loads vertex and fragment shaders
//-----------------------------------------------------------------------------
bool ShaderProgram::loadShaders(const char* vsFilename, const char* fsFilename)
{
	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);

	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);
	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);

	// 5. Create shader program and link shaders to program
	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);

	checkCompileErrors(mHandle, PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);
	mUniformLocations.clear();
	return true;
}

// Activate the shader program
void ShaderProgram::use()
{
	if (mHandle > 0)
	{
		glUseProgram(mHandle);
	}
}

// Opens and reads contents of ASCII file to a string.  Returns the string.
// Not good for very large files.
string ShaderProgram::fileToString(const string& filename)
{
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);

		if (!file.fail())
		{
			// Using a std::stringstream is easier than looping through each line of the files
			ss << file.rdbuf();
		}
		file.close();
	}
	catch (std::exception ex)
	{
		std::cerr << "Error reading shader filename!" << std::endl;
	}

	return ss.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;
	if (type == PROGRAM)
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);

			string errorLog(length, ' ');
			glGetProgramInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "Error!Program failed to link" << errorLog << std::endl;
		}
	}
	else 
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);

			string errorLog(length, ' ');
			glGetProgramInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "Error!Vertex Shader failed to link" << errorLog << std::endl;
		}
	}
}

// Returns the uniform identifier given it's string name.
// NOTE: Shader must be currently active first.
GLint ShaderProgram::getUniformLocations(const GLchar* name)
{
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);
	// Only need to query the shader program IF it doesn't already exist.
	if (it == mUniformLocations.end())
	{
		// Find it and add it to the map
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}
	// Return it
	return mUniformLocations[name];
}

GLuint ShaderProgram::getProgram()const
{
	return mHandle;
}

// Sets a glm::vec2 shader uniform
void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
	GLint loc = getUniformLocations(name);
	glUniform2f(loc, v.x, v.y);
}
// Sets a glm::vec3 shader uniform
void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
	GLint loc = getUniformLocations(name);
	glUniform3f(loc, v.x, v.y, v.z);
}
// Sets a glm::vec4 shader uniform
void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
	GLint loc = getUniformLocations(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m)
{
	GLint loc = getUniformLocations(name);
	// loc = location of uniform in shader
	// count = how many matrices (1 if not an array of mats)
	// transpose = False for opengl because column major
	// value = the matrix to set for the uniform
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}