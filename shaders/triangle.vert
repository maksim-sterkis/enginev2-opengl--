#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 aInstanceMatrix; // Takes up locations 2, 3, 4, 5

out vec2 TexCoord;

uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    gl_Position = u_proj * u_view * aInstanceMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
