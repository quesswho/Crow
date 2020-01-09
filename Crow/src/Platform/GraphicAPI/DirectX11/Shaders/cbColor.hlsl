R"(
#shader fragment

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 PSmain(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}

#shader vertex

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer u_Color : register(b0)
{
	float4 UniformColor;
};

VS_OUTPUT VSmain(float4 pos : POSITION)
{
    VS_OUTPUT output;
	output.pos = pos;
    output.color = UniformColor;
    return output;
}

)"