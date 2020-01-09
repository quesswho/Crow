R"(
#shader fragment

#version 330 core

out vec4 pixel;

uniform vec3 u_Color;

void main()
{
    pixel = vec4(u_Color.xyz, 1.0f);
}

#shader vertex

#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_Projection = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Model = mat4(1.0f);

void main()
{
    gl_Position =  u_Projection * u_View * u_Model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
}
)"