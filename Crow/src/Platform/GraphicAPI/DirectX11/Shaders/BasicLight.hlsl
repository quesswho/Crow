R"(
#shader fragment

Texture2D texture0;
SamplerState sampleState;

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};


float4 PSmain(VS_OUTPUT input) : SV_TARGET
{
    //float brightness = 1.0 / length(input.pos. - lightpos.xy + float2(0.2f, 0.5f));
    //float3 result = input.color.xyz * brightness;
    //return saturate(float4(lightpos.xy, brightness, 1.0f));
	float4 result = texture0.Sample(sampleState, input.uv);
	return result;
}

#shader vertex

struct VS_INPUT
{
    float3 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

cbuffer u_VP : register(b0)
{
    float4x4 vp;
}

cbuffer u_Model : register(b1)
{
    float4x4 model;
}

VS_OUTPUT VSmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(mul(float4(input.pos, 1.0f), model), vp);
    output.uv = input.uv;

    return output;
}

)"