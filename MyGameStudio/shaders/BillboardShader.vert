#version 330 core
layout (location = 0) in vec2 quadPos;

uniform vec3 centerWorld;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main() {
    // Extract right and up vectors from view matrix
    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 up    = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 worldPos = centerWorld + quadPos.x * right + quadPos.y * up;

    gl_Position = projection * view * vec4(worldPos, 1.0);
    texCoord = quadPos + vec2(0.5);
}