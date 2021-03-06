#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include <string>
#include <map>
#include "GL/glew.h"
#include "glm/glm.hpp"

using std::string;

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void use();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);
	void setUniform(const GLchar* name, const GLfloat& m);
	void setUniformSampler(const GLchar* name, const GLint& m);
	void setUniform(const GLchar* name, const GLint m);
	GLuint getProgram()const;

private:
	string fileToString(const string& filename);    
	void checkCompileErrors(GLuint shader, ShaderType type);

	GLint getUniformLocations(const GLchar* name);

	GLuint mHandle;
	std::map<string, GLint> mUniformLocations;  
};

#endif // SHADER_PROGRAM_H