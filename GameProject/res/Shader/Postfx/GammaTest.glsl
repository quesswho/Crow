#shader fragment

#version 330 core

out vec4 pixel;

in vec2 f_TexCoord;

uniform sampler2D u_Texture;

uniform float u_Gamma = 2.2;

void main()
{
	vec3 result = pow(texture(u_Texture, f_TexCoord).rgb, vec3(u_Gamma));
    pixel = vec4(result, 1.0f);
}

#shader vertex

#version 330 core
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 f_TexCoord;

void main()
{
	f_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}