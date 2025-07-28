#version 330 core
in vec4 vertexColor;
in vec2 texCoord;

out vec4 FragColor;

uniform bool useVertexColor;
uniform sampler2D ourTexture;

void main()
{
	if (useVertexColor)
		FragColor = vertexColor;
	else	
		FragColor = texture(ourTexture, texCoord);
}