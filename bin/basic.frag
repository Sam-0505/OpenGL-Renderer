#version 330 core

in vec2 TexCoord;
out vec4 frag_color;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

//when there was only one texture
/*
void main()
{
	frag_color=texture(myTexture1,TexCoord);
}
*/

//when two textures are mixed
void main()
{
	frag_color=mix(texture(myTexture1,TexCoord),texture(myTexture2,TexCoord),0.5);
}