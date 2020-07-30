#version 330 core

layout(location = 0) in vec3 pos;// this 0 is linked to glVertexAttribPointer(0,..)
layout(location=1) in vec2 texCoord;

out vec2 TexCoord;

void main()
{
	gl_Position=vec4(pos.x,pos.y,pos.z,1.0);
	 TexCoord=texCoord;
}

 