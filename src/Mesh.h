#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "GL/glew.h"	// Important - this header must come before glfw3 header
#include "glm/glm.hpp"


struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
};

class Mesh
{
public:

	Mesh();
	~Mesh();

	bool loadOBJ(const std::string& filename);
	void draw();
	

private:

	void initBuffers();
	std::vector<std::string> split(std::string vert, std::string del);

	bool mLoaded;
	std::vector<Vertex> mVertices;	GLuint mVBO, mVAO;
};
#endif //MESH_H

