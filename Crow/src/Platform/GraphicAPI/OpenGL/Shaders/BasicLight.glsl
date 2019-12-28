R"(
#shader fragment

#version 330 core

out vec4 pixel;

struct Light {
	vec4 color;
	vec2 pos;
};

uniform Light u_Light;

in vec2 f_pos;
in vec3 f_color;

void main()
{
	float brightness = 1.0 / length(f_pos - u_Light.pos);
	vec3 result = f_color * u_Light.color.rgb * brightness;
    pixel = vec4(result, 1.0f);
}

#shader vertex

#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;


uniform mat4 u_Projection = mat4(1.0f);
uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);

out vec2 f_pos;
out vec3 f_color;

void main()
{
	
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0);
	f_pos = a_Position.xy;
	f_color = a_Color.xyz;
}
)"