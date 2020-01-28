R"(

#shader fragment

Texture2D texture0;
SamplerState sampleState;

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer u_Color : register(b1)
{
	float4 c_Color;
}

float4 PSmain(VS_OUTPUT input) : SV_TARGET
{
	return float4(c_Color.xyz, texture0.Sample(sampleState, input.uv).r);
}

#shader vertex

struct VS_INPUT
{
    float2 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer u_Projection : register(b0)
{
    float4x4 projection;
}


VS_OUTPUT VSmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos.x, input.pos.y, 0.0f, 1.0f), projection);
    output.uv = input.uv;

    return output;
}
)"