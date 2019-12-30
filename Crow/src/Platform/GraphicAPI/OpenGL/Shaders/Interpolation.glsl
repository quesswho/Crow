R"(
#shader fragment

#version 330 core

out vec4 pixel;

in vec3 f_pos;

void main()
{
    pixel = vec4(f_pos.x, f_pos.y, f_pos.z, 1.0f);
}

#shader vertex

#version 330 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_Projection = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Model = mat4(1.0f);

out vec3 f_pos;

void main()
{
	
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
	f_pos = normalize(a_Position.xyz);
}
)"