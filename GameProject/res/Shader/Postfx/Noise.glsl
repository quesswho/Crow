#shader fragment

#version 330 core

out vec4 pixel;

uniform vec2 u_Mousepos;
uniform vec2 u_Resolution;

float random(vec2 xy)
{
    return fract(sin(dot(xy, u_Mousepos)) * 314159);
}

float noise (vec2 xy) {
    vec2 i = floor(xy);
    vec2 f = fract(xy);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f*f*(3.0-2.0*f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main()
{
	float result = noise(10.0f * gl_FragCoord.xy / u_Resolution.xy);
    pixel = vec4(result, result, result, 1.0f);
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