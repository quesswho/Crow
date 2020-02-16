#shader fragment

Texture2D texture0;
SamplerState sampleState;

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer u_Gamma : register(b0)
{
	float c_Gamma;
}

float4 PSmain(VS_OUTPUT input) : SV_TARGET
{
	return float4(pow(texture0.Sample(sampleState, input.uv).rgb, c_Gamma), 1.0f);
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

VS_OUTPUT VSmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 0.0f, 1.0f);
    output.uv = input.uv;

    return output;
}