#version 330 core

layout(location = 0) in vec3 pos;// this 0 is linked to glVertexAttribPointer(0,..)
layout(location=1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

void main()
{
	gl_Position=projection * view * model *vec4(pos.x,pos.y,pos.z,1.0);
	 TexCoord=texCoord;
}

 