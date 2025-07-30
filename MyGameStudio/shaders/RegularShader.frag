#version 330 core

// Structs
struct TexCoords
{
	vec2 baseColor;
	vec2 normal;
	vec2 metallicRoughness;
	vec2 occlusion;
	vec2 emissive;
};

struct Material
{
	sampler2D baseColorTex;
	sampler2D normalTex;
	sampler2D metallicRoughnessTex;
	sampler2D occlusionTex;
	sampler2D emissiveTex;

	float metallicFactor;
	float roughnessFactor;
	float occlusionFactor;
	vec3 emissiveFactor; 
};

// In data
in vec3 normal;
in vec3 fragPos;
in vec4 vertexSolidColor;

in TexCoords texCoords;

// Out data
out vec4 FragColor;

// Uniforms
// Material rendering properties
uniform bool useVertexColor;
uniform Material materialProps;

// Light
uniform vec3 ambientColor;
uniform float ambientFactor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightStrength;

void main()
{
	// Get object color	
	vec4 objectColor;	

	if (useVertexColor)	
		objectColor = vertexSolidColor;
	else
		objectColor = texture(materialProps.baseColorTex, texCoords.baseColor);	


	// Lighting calculations
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	vec3 ambientLight = ambientFactor * ambientColor;	
	vec3 diffuseLight = max(dot(norm, lightDir), 0.0) * lightColor * lightStrength;	
	vec3 specularLight = pow(max(dot(viewDir, reflectDir), 0.0), 32) * materialProps.occlusionFactor * lightStrength * lightColor;	

	
	// Apply lighting
	FragColor = vec4(ambientLight + diffuseLight + specularLight, 1.0) * objectColor;		
}