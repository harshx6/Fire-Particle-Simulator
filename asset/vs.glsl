#version 330 core
layout (location = 0) in vec3 vertex; 
layout (location = 1) in vec2 Tex; 

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;

uniform mat4 model;

uniform vec4 color;

void main()
{

    TexCoords = Tex;

    float scale = 1.0f;

    ParticleColor = color;

    gl_Position = projection * view * model * vec4(vertex, 1.0f);
}