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

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightStrength;

void main()
{
	// Get object color	
	vec4 objectColor;	

	if (useVertexColor)	
		objectColor = vertexColor;
	else
		objectColor = texture(ourTexture, texCoord);	


	// Lighting calculations
	float specularFactor = 0.7;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	vec3 ambientLight = ambientFactor * ambientColor;	
	vec3 diffuseLight = max(dot(norm, lightDir), 0.0) * lightColor * lightStrength;	
	vec3 specularLight = pow(max(dot(viewDir, reflectDir), 0.0), 32) * specularFactor * lightStrength * lightColor;	

	
	// Apply lighting
	FragColor = vec4(ambientLight + diffuseLight + specularLight, 1.0) * objectColor;		
}