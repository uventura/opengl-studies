#version 330 core
out vec4 frag_color;

in vec4 positions;

void main()
{
    frag_color = positions;
}
