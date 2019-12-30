R"(
#shader fragment

#version 330 core

out vec4 pixel;

in vec4 f_Color;

void main()
{
    pixel = f_Color;
}

#shader vertex

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

uniform mat4 u_Projection = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Model = mat4(1.0f);

out vec4 f_Color;

void main()
{
	f_Color = a_Color;
    gl_Position =  u_Projection * u_View * u_Model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
}
)"