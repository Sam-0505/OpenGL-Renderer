#version 330 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
};

struct Material
{
	vec3 ambient;
	vec3 specular;
	float shininess;
	sampler2D diffuseMap;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 frag_color;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
	//ambient
	vec3 ambient=light.ambient*material.ambient;

	//diffuse
	vec3 normal=normalize(Normal);
	vec3 lightDir=normalize(light.position-FragPos);
	float NdotP=max(dot(normal,lightDir),0.0f);
	vec3 diffuse=NdotP*vec3(texture(material.diffuseMap,TexCoord))*light.diffuse;

	//specular
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 halfvector=normalize(lightDir+viewDir);
	float HdotN=max(dot(normal,halfvector),0.0f);
	vec3 specular=light.specular*material.specular*pow(HdotN,material.shininess);

	frag_color=vec4((ambient+diffuse+specular),1.0f);
}