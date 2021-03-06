R"(
#shader fragment

#version 330 core

out vec4 pixel;

in vec2 f_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    pixel = texture(u_Texture, f_TexCoord);
}

#shader vertex

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 f_TexCoord;

uniform mat4 u_VP = mat4(1.0f);
uniform mat4 u_Model = mat4(1.0f);

void main()
{
	f_TexCoord = a_TexCoord;
    gl_Position = u_VP * u_Model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0f);
}
)"