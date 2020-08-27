#version 330 core

struct DirLight
{
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
	vec3 direction;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;

	float constant;
	float linear;
	float quad;
};

struct SpotLight
{
	vec3 position;
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
	vec3 direction;
	float cosInnerCone;
	float cosOuterCone;
	int on;

	float constant;
	float linear;
	float quad;
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

uniform DirLight dlight;
uniform PointLight plight;
uniform SpotLight slight;
uniform Material material;
uniform vec3 viewPos;

vec3 calcDirLight(vec3 normal);
vec3 calcPointLight(vec3 normal);
vec3 calcSpotLight(vec3 normal);

void main()
{
	//diffuse
	vec3 normal=normalize(Normal);
	vec3 result=calcDirLight(normal)+calcPointLight(normal);
	frag_color=vec4(result,1.0f);
}

vec3 calcDirLight(vec3 normal)
{
	//ambient
	vec3 ambient=dlight.ambient*material.ambient*vec3(texture(material.diffuseMap,TexCoord));

	//diffuse
	vec3 lightDir=dlight.direction;
	float NdotL=max(dot(normal,lightDir),0.0f);
	vec3 diffuse=NdotL*vec3(texture(material.diffuseMap,TexCoord))*dlight.diffuse;

	//specular
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 halfvector=normalize(lightDir+viewDir);
	float HdotN=max(dot(normal,halfvector),0.0f);
	vec3 specular=dlight.specular*material.specular*pow(HdotN,material.shininess);

	return ambient+diffuse+specular;
}

vec3 calcPointLight(vec3 normal)
{
	//ambient
	vec3 ambient=plight.ambient*material.ambient*vec3(texture(material.diffuseMap,TexCoord));

	float lightDist=length(plight.position-FragPos);
	float atten=1/(plight.constant+plight.linear*lightDist+plight.quad*lightDist*lightDist);

	//diffuse
	vec3 lightDir=normalize(plight.position-FragPos);
	float NdotL=max(dot(normal,lightDir),0.0f);
	vec3 diffuse=NdotL*vec3(texture(material.diffuseMap,TexCoord))*dlight.diffuse*atten;

	//specular
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 halfvector=normalize(lightDir+viewDir);
	float HdotN=max(dot(normal,halfvector),0.0f);
	vec3 specular=dlight.specular*material.specular*pow(HdotN,material.shininess)*atten;

	return ambient+diffuse+specular;
}

vec3 calcSpotLight(vec3 normal)
{
	//ambient
	vec3 ambient=slight.ambient*material.ambient*vec3(texture(material.diffuseMap,TexCoord));
	
	//diffuse
	vec3 lightDir=normalize(slight.position-FragPos);
	vec3 spotDir=normalize(slight.direction);
	float NdotL=max(dot(normal,lightDir),0.0f);
	vec3 diffuse=NdotL*vec3(texture(material.diffuseMap,TexCoord))*dlight.diffuse;

	//specular
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 halfvector=normalize(lightDir+viewDir);
	float HdotN=max(dot(normal,halfvector),0.0f);
	vec3 specular=dlight.specular*material.specular*pow(HdotN,material.shininess);

	float cosDir=dot(lightDir,spotDir);
	float spotIntensity=smoothstep(slight.cosInnerCone,slight.cosOuterCone,cosDir);
	
	float lightDist=length(slight.position-FragPos);
	float atten=1/(slight.constant+slight.linear*lightDist+slight.quad*lightDist*lightDist);

	diffuse*=atten*spotIntensity;
	specular*=atten*spotIntensity;

	return ambient+diffuse+specular;
}