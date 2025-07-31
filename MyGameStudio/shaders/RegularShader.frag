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

struct DirectionalLight
{
	vec3 color;	
	vec3 direction;
	float intensity;
};

struct PointLight
{
	vec3 pos;
	vec3 color;
	float constant;	
	float linear;
	float quadratic;
};

struct Spotlight
{
	vec3 pos;
	vec3 color;
	vec3 direction;
	float innerCutoffAngle;
	float outerCutoffAngle;
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
uniform vec3 viewPos;

uniform vec3 ambientColor;
uniform float ambientIntensity;

#define MAX_DIR_LIGHTS 16
#define MAX_POINT_LIGHTS 128
#define MAX_SPOTLIGHTS 128

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform Spotlight spotlights[MAX_SPOTLIGHTS];

uniform float dirLightsCount;
uniform float pointLightsCount;
uniform float spotlightsCount;

// Function signatures
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

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
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 ambientLight = ambientIntensity * ambientColor;

	vec3 totalLight = vec3(0.0f);

	totalLight += ambientLight;

	for (int i = 0; i < dirLightsCount; ++i)
		totalLight += CalcDirLight(dirLights[i], norm, viewDir);
	
	for (int i = 0; i < pointLightsCount; ++i)
		totalLight += CalcPointLight(pointLights[i], norm, fragPos, viewDir);

	for (int i = 0; i < spotlightsCount; ++i)
		totalLight += CalcSpotlight(spotlights[i], norm, fragPos, viewDir);
	
	// Apply lighting
	FragColor = vec4(totalLight * objectColor.rgb, objectColor.a);		
}

// Function definitions
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	return vec3(0,0,0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	vec3 diffuseLight = max(dot(normal, lightDir), 0.0) * light.color;	
	vec3 specularLight = pow(max(dot(viewDir, reflectDir), 0.0), 32) * 0.6 * light.color;	
	
	return vec3(0,0,0);
}

vec3 CalcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	return vec3(0,0,0);
}