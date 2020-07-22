#version 330 core

layout(location = 0) in vec3 pos;// this 0 is linked to glVertexAttribPointer(0,..)

uniform vec2 posOffset;

void main()
{
	gl_Position=vec4(pos.x+posOffset.x,pos.y+posOffset.y,pos.z,1.0);
};
 