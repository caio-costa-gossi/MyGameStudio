#version 330 core
in vec4 vertexColor;
in vec2 texCoord;

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform bool useVertexColor;
uniform sampler2D ourTexture;

uniform vec3 ambientColor;
uniform float ambientFactor;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	// Get object color	
	vec4 objectColor;	

	if (useVertexColor)	
		objectColor = vertexColor;
	else
		objectColor = texture(ourTexture, texCoord);	


	// Lighting calculations
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	
	vec3 diffuseLight = max(dot(norm, lightDir), 0.0) * lightColor;	
	vec3 ambientLight = ambientFactor * ambientColor;	

	
	// Apply lighting
	FragColor = vec4(ambientLight + diffuseLight, 1.0) * objectColor;		
}