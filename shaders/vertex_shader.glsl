#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 acolor;
layout (location = 2) in vec2 atex;

out vec3 ourColor;
out vec2 tex_color;

uniform vec3 deltaPos;
vec3 positions;

uniform mat4 transform;

void main()
{
    positions = vec3(apos.x + deltaPos.x, apos.y, apos.z);
    gl_Position = transform * vec4(positions, 1.0);

    ourColor = acolor + deltaPos;
    tex_color = atex;
}
