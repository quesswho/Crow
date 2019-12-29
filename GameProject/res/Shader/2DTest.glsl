#shader fragment

#version 330 core

out vec4 pixel;

in vec4 f_color;

void main()
{
    pixel = f_color;
}

#shader vertex

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_color;

uniform mat3 u_Model = mat3(1.0f);

out vec4 f_color;

void main()
{
	f_color = a_color;
	vec3 pos = u_Model * a_Position;
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}