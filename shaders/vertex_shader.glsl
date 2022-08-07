#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 acolor;

out vec3 ourColor;

uniform vec3 deltaPos;
vec3 positions;

void main()
{
    positions = vec3(apos.x + deltaPos.x, apos.y, apos.z);
    gl_Position = vec4(positions, 1.0);

    ourColor = acolor + deltaPos;
}
