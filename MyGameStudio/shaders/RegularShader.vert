#version 330 core
// Vertex data layout
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

layout (location = 3) in vec2 aBaseColorTexCoord;
layout (location = 4) in vec2 aNormalTexCoord;
layout (location = 5) in vec2 aMetallicRoughnessTexCoord;
layout (location = 6) in vec2 aOcclusionTexCoord;
layout (location = 7) in vec2 aEmissiveTexCoord;

// Structs
struct TexCoords
{
	vec2 baseColor;
	vec2 normal;
	vec2 metallicRoughness;
	vec2 occlusion;
	vec2 emissive;
};

// Out data
out vec3 normal;
out vec3 fragPos;
out vec4 vertexSolidColor;
out TexCoords texCoords;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

void main()
{
	normal = normalMatrix * aNormal;	
	fragPos = vec3(model * vec4(aPos, 1.0f));
	vertexSolidColor = aColor;

	texCoords.baseColor = aBaseColorTexCoord;
	texCoords.normal = aNormalTexCoord;
	texCoords.metallicRoughness = aMetallicRoughnessTexCoord;
	texCoords.occlusion = aOcclusionTexCoord;
	texCoords.emissive = aEmissiveTexCoord;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);	
}