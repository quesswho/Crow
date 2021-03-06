R"(
#shader fragment

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

float4 PSmain(VS_OUTPUT input) : SV_TARGET
{
    // return interpolated color
    return input.color;
}

#shader vertex

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

cbuffer u_Color : register(b0)
{
	float3 UniformColor;
};

VS_OUTPUT VSmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 1.0f);
    output.color = float4(UniformColor.xyz, 1.0f);
    return output;
}

)"