#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

layout (location = 3) in vec2 aBaseColorTexCoord;
layout (location = 4) in vec2 aNormalTexCoord;
layout (location = 5) in vec2 aMetallicRoughnessTexCoord;
layout (location = 6) in vec2 aOcclusionTexCoord;
layout (location = 7) in vec2 aEmissiveTexCoord;

out vec4 vertexColor;
out vec2 texCoord;

out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrix;

void main()
{
	normal = normalMatrix * aNormal;	
	fragPos = vec3(model * vec4(aPos, 1.0f));

	vertexColor = aColor;
	texCoord = aBaseColorTexCoord;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);	
}