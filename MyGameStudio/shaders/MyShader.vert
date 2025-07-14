#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 texCoord;

uniform vec3 posOffset;

void main()
{
	gl_Position = vec4(aPos.x + posOffset.x, aPos.y + posOffset.y, aPos.z + posOffset.z, aPos.w);
	vertexColor = vec4(aColor);
	texCoord = aTexCoord;
}