#shader fragment

#version 330 core
out vec4 FragColor;

uniform vec3 color = vec3(0.6f, 0.6f, 0.6f); // Grey default color

void main()
{
    FragColor = vec4(color.xyz, 1.0f);
}

#shader vertex

#version 330 core
layout (location = 0) in vec3 a_Position;

void main()
{
    gl_Position = vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
}