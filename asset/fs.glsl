#version 330 core

in vec2 TexCoords;

in vec4 ParticleColor;

out vec4 color;

uniform sampler2D sprite;

uniform float fTime;

void main()
{
    color = (texture(sprite, TexCoords) * ParticleColor) * fTime;
    //color = ParticleColor;
    color.w = 0.7f;
} 