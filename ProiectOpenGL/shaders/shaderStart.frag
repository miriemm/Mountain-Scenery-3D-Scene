#version 410 core

struct FogParameters
{
	vec3 color;
	float density;
	
	bool isEnabled;
};

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;
uniform sampler2D shadowMap;

// fog
uniform FogParameters fogParameters;

//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;


void computeLightComponents()
{	

	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}

float computeShadow() 
{ 
    //prespective divide
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	//tranform to [0,1] range
	normalizedCoords = normalizedCoords * 0.5 + 0.5;
	//closest depth value from light's perspective
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
	//depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;
	//chech if current fragment position is in shadow
	float bias = max(0.05f * (1.0f - dot(fNormal, lightDir)), 0.005f);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	return shadow;
}



float getFogFactor(FogParameters params, float fogCoordinate)
{
	float result = 0.0f;

	result = exp(-pow(params.density * fogCoordinate, 2.0f));
	
	result = clamp(result, 0.0f, 1.0f);
	return result;
}


void main() 
{
	computeLightComponents();
	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;
	
	float shadow = computeShadow(); 
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
	
	if(fogParameters.isEnabled)
	{
	  float fogCoordinate = length(fPosEye);
      fColor = mix(vec4(fogParameters.color, 1.0f), vec4(color, 1.0f), getFogFactor(fogParameters, fogCoordinate));
	}
	else
	{
	  fColor = vec4(color, 1.0f);
	}
}
