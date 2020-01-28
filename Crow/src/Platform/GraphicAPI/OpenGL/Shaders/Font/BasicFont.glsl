R"(
#shader fragment

#version 330 core

in vec2 f_TexCoords;

out vec4 color;

uniform sampler2D u_Texture;

void main()
{    
    color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture, f_TexCoords).r);
}  

#shader vertex

#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 f_TexCoords;

uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * vec4(vertex.xy, 0.0, 1.0);
    f_TexCoords = vertex.zw;
} 
)"