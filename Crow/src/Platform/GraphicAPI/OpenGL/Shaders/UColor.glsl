R"(
#shader fragment

#version 330 core

out vec4 pixel;

uniform vec4 u_Color;

void main()
{
    pixel = u_Color;
}

#shader vertex

#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_MVP = mat4(1.0f);

void main()
{
    gl_Position = u_MVP * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
}
)"