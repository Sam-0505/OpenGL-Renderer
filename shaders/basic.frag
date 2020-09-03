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

uniform int Dir_num;
uniform int Point_num;
uniform int Spot_num;
uniform DirLight dlight[50];
uniform PointLight plight[50];
uniform SpotLight slight[50];
uniform Material material;
uniform vec3 viewPos;

vec3 calcDirLight(vec3 normal);
vec3 calcPointLight(vec3 normal);
vec3 calcSpotLight(vec3 normal);

void main()
{
	//diffuse 
	vec3 normal=normalize(Normal);
	vec3 result=calcPointLight(normal)+calcSpotLight(normal)+calcDirLight(normal);
	frag_color=vec4(result,1.0f);
}

vec3 calcDirLight(vec3 normal)
{
	vec3 effect;
	for(int i=0;i<Dir_num;i++)
	{
		//ambient
		vec3 ambient=dlight[i].ambient*material.ambient*vec3(texture(material.diffuseMap,TexCoord));

		//diffuse
		vec3 lightDir=dlight[i].direction;
		float NdotL=max(dot(normal,lightDir),0.0f);
		vec3 diffuse=NdotL*vec3(texture(material.diffuseMap,TexCoord))*dlight[i].diffuse;

		//specular
		vec3 viewDir=normalize(viewPos-FragPos);
		vec3 halfvector=normalize(lightDir+viewDir);
		float HdotN=max(dot(normal,halfvector),0.0f);
		vec3 specular=dlight[i].specular*material.specular*pow(HdotN,material.shininess);

		effect+=ambient+specular+diffuse;
	}
	return effect;
}

vec3 calcPointLight(vec3 normal)
{
	vec3 effect;
	for(int i=0;i<Point_num;i++)
	{
	//ambient
	vec3 ambient=plight[0].ambient*material.ambient*vec3(texture(material.diffuseMap,TexCoord));

	float lightDist=length(plight[0].position-FragPos);
	float atten=1/(plight[0].constant+plight[0].linear*lightDist+plight[0].quad*lightDist*lightDist);

	//diffuse
	vec3 lightDir=normalize(plight[0].position-FragPos);
	float NdotL=max(dot(normal,lightDir),0.0f);
	vec3 diffuse=NdotL*vec3(texture(material.diffuseMap,TexCoord))*plight[0].diffuse*atten;

	//specular
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 halfvector=normalize(lightDir+viewDir);
	float HdotN=max(dot(normal,halfvector),0.0f);
	vec3 specular=plight[0].specular*material.specular*pow(HdotN,material.shininess)*atten;

	effect+=ambient+specular+diffuse;
	}
	return effect;
}

vec3 calcSpotLight(vec3 normal)
{
	vec3 effect;
	for(int i=0;i<Spot_num;i++)
	{
		//ambient
		vec3 ambient=slight[i].ambient*material.ambient*vec3(texture(material.diffuseMap,TexCoord));
	
		//diffuse
		vec3 lightDir=normalize(slight[i].position-FragPos);
		vec3 spotDir=normalize(slight[i].direction);
		float NdotL=max(dot(normal,lightDir),0.0f);
		vec3 diffuse=NdotL*vec3(texture(material.diffuseMap,TexCoord))*slight[i].diffuse;

		//specular
		vec3 viewDir=normalize(viewPos-FragPos);
		vec3 halfvector=normalize(lightDir+viewDir);
		float HdotN=max(dot(normal,halfvector),0.0f);
		vec3 specular=slight[i].specular*material.specular*pow(HdotN,material.shininess);

		float cosDir=dot(lightDir,spotDir);
		float spotIntensity=smoothstep(slight[i].cosInnerCone,slight[i].cosOuterCone,cosDir);
	
		float lightDist=length(slight[i].position-FragPos);
		float atten=1/(slight[i].constant+slight[i].linear*lightDist+slight[i].quad*lightDist*lightDist);

		diffuse*=atten*spotIntensity;
		specular*=atten*spotIntensity;
	
		effect+=ambient+diffuse+specular;
	}
	return effect;
}