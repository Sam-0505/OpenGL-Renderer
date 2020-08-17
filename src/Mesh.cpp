#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

Mesh::Mesh()
	:mLoaded(false),
	mVAO(0),
	mVBO(0)
{
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteVertexArrays(1, &mVBO);
}

bool Mesh::loadOBJ(const std::string& filename)
{
	std::vector<glm::vec3> vert_pos;
	std::vector<glm::vec2> tex_coords;
	std::vector<glm::vec3> vert_nor;

	std::vector<GLuint> vert_pos_indices;
	std::vector<GLuint> tex_coord_indices;
	std::vector<GLuint> vert_nor_indices;

	std::vector<Vertex> vertices;
	std::ifstream in_file(filename, std::ios::in);
	std::string line;
	try
	{
		if (filename.find(".obj") == std::string::npos)
		{
			std::cerr << "File cannot be found" << std::endl;
		}
		else
		{
			std::cout << "File in else" << std::endl;
			if (!in_file.is_open())
			{
				std::cerr << "File cannot be opened" << std::endl;
			}
			else
			{
				while (std::getline(in_file, line))
				{
					std::string prefix = line.substr(0, 2);
					if (prefix == "v ")
					{
						const char* lineBuff = line.c_str();
						glm::vec3 tempPos;
						int match = sscanf_s(lineBuff, "v %f %f %f", &tempPos.x, &tempPos.y, &tempPos.z);
						if (match != 3)
							std::cout << "Failed to parse OBJ file using our very simple OBJ loader Pos(v)" << std::endl;

						vert_pos.push_back(tempPos);
					}
					else if (prefix == "vt")
					{
						const char* lineBuff = line.c_str();
						float gf;
						glm::vec2 texCoord;
						int match = sscanf_s(lineBuff, "vt %f %f %f", &texCoord.x, &texCoord.y, &gf);
						if (match != 3)
							std::cout << "Failed to parse OBJ file using our very simple OBJ loader(vt)" << std::endl;
						tex_coords.push_back(texCoord);
					}
					else if (prefix == "vn")
					{
						const char* lineBuff = line.c_str();
						glm::vec3 tempNor;
						int match = sscanf_s(lineBuff, "vn %f %f %f", &tempNor.x, &tempNor.y, &tempNor.z);
						if (match != 3)
							std::cout << "Failed to parse OBJ file using our very simple OBJ loader(vn)" << std::endl;
						vert_nor.push_back(tempNor);
					}
					else if (prefix == "f ")
					{
						const char* lineBuff = line.c_str();
						GLint p1, p2, p3, t1, t2, t3, n1, n2, n3;
						int match=sscanf_s(lineBuff, "f %i/%i/%i %i/%i/%i %i/%i/%i", &p1, &t1, &n1, &p2, &t2, &n3, &p3, &t3, &n3);
						if (match != 9)
							std::cout << "Failed to parse OBJ file using our very simple OBJ loader(f)" << std::endl;
						vert_pos_indices.push_back(p1);
						vert_pos_indices.push_back(p2);
						vert_pos_indices.push_back(p3);

						tex_coord_indices.push_back(t1);
						tex_coord_indices.push_back(t2);
						tex_coord_indices.push_back(t3);
					}
				}
				for (int i = 0; i < vert_pos_indices.size(); i++)
				{

					Vertex tempVertex;
					tempVertex.position = vert_pos[vert_pos_indices[i] - 1];
					tempVertex.texCoords = tex_coords[tex_coord_indices[i] - 1];

					mVertices.push_back(tempVertex);
				}
				in_file.close();
				initBuffers();

				std::cout << "mLoaded is now true" << std::endl;
				return(mLoaded = true);
			}
		}
	}
	catch (const std::bad_alloc&) {
		std::cerr << "Catch me hu bhai" << std::endl;
		return -1;
	}
	return false;
}

void Mesh::initBuffers()
{
	glGenBuffers(1, &mVBO);// Generate an empty vertex buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);// "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);// copy the data from CPU to GPU

	// The vertex array object (VAO) is a little descriptor that defines which data from vertex buffer objects should be used as input 
	// variables to vertex shaders.
	glGenVertexArrays(1, &mVAO);// Tell OpenGL to create new Vertex Array Object
	glBindVertexArray(mVAO);// Make it the current one

	// Position attribute, identified as "0"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);//(AttribIndex,No. of components,Data type,Normalize?,Stride Length,Offset
	glEnableVertexAttribArray(0);//Enable first Attribute or attribute"0"

	// Texture Coord attribute, identified as "1"
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(GL_FLOAT)*3));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Mesh::draw()
{
	if (!mLoaded)
		return;
	else
	{
		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
		glBindVertexArray(0);
	}
}

