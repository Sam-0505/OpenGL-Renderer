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
					std::stringstream ss(line);
					std::string cmd;
					ss >> cmd;
					if (cmd == "v")
					{
						glm::vec3 tempPos;
						int dim=0;
						while (dim<3)
						{
							ss >> tempPos[dim];
							dim++; 
						}
						vert_pos.push_back(tempPos);
					}
					else if (cmd == "vt")
					{
						glm::vec2 texCoord;
						int dim = 0;
						while (dim < 2)
						{
							ss >> texCoord[dim];
							dim++;
						}
						tex_coords.push_back(texCoord);
					}
					else if (cmd == "vn")
					{
						glm::vec3 tempNor;
						int dim = 0;
						while (dim < 3)
						{
							ss >> tempNor[dim];
							dim++;
						}
						glm::vec3 normal = glm::normalize(tempNor);
						vert_nor.push_back(normal);
					}
					else if (cmd == "f")
					{
						std::string vert;
						while (ss >> vert)
						{
							std::vector<std::string> indices = split(vert, "/");
							if (indices.size() > 0)
							{
								vert_pos_indices.push_back(std::stoi(indices[0]));
							}
							if (indices.size() > 1)
							{
								tex_coord_indices.push_back(std::stoi(indices[1]));
							}
							if (indices.size() > 2)
							{
								vert_nor_indices.push_back(std::stoi(indices[2]));
							}
						}
					}
				}
				for (int i = 0; i < vert_pos_indices.size(); i++)
				{

					Vertex tempVertex;
					if(vert_pos.size()>0)
						tempVertex.position = vert_pos[vert_pos_indices[i] - 1];
					if(vert_nor.size()>0)
						tempVertex.normal = vert_nor[vert_nor_indices[i] - 1];
					if(tex_coords.size()>0)
						tempVertex.texCoords=tex_coords[tex_coord_indices[i] - 1];

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

std::vector<std::string> Mesh::split(std::string vert, std::string del)
{
	int pos = 0;
	std::vector<std::string> res;
	while(true)
	{ 
		pos = vert.find(del);
		if (pos == -1)
		{
			res.push_back(vert.substr(0,pos));
			return res;
		}
		res.push_back(vert.substr(0, pos));
		vert = vert.substr(pos+1,vert.size()-pos-1);
	}
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

