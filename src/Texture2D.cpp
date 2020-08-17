#include  "Texture2D.h"
#include <iostream>
#include<cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture2D::Texture2D()
	:mTexture(0)
{
}
Texture2D:: ~Texture2D()
{
	glDeleteTextures(1, &mTexture);
}

bool Texture2D::loadTexture(const string& filename, bool generateMipMaps)
{
	int width, height, components;

	// Use stbi image library to load our image
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (imageData == NULL)
	{
		string s=stbi_failure_reason();
		std::cout << s << std::endl;
		std::cerr << "Error loading texture" << filename << "'" << std::endl;
		return false;
	}

	//invert image
	int widthInbytes = width * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	for (int row = 0; row < height / 2; row++)
	{
		top = imageData + row * widthInbytes;
		bottom= imageData + (height-row-1)* widthInbytes;
		for (int col = 0; col < widthInbytes; col++)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	// all upcoming GL_TEXTURE_2D operations will affect our texture object (mTexture)
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	// Set the texture wrapping/filtering options (on the currently bound texture object)
	// GL_CLAMP_TO_EDGE
	// GL_REPEAT
	// GL_MIRRORED_REPEAT
	// GL_CLAMP_TO_BORDER
	// GL_LINEAR
	// GL_NEAREST[
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,imageData);

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

// Bind the texture unit passed in as the active texture in the shader
void Texture2D::bind(GLuint textUnit)
{
	glActiveTexture(GL_TEXTURE0 + textUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture2D::unbind(GLuint textUnit)
{
	glActiveTexture(GL_TEXTURE0 + textUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}