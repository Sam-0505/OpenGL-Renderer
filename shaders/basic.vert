#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;// this 0 is linked to glVertexAttribPointer(0,..)
layout(location = 2) in vec2 texCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

void main()
{
	gl_Position=projection * view * model *vec4(pos.x,pos.y,pos.z,1.0);
	 TexCoord=texCoord;
	 Normal=normal;
	 FragPos=vec3(model*vec4(pos,1.0f));
}

