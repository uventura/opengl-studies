#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec2 atex;

out vec2 tex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(apos, 1.0);

    tex_color = atex;
}
