R"(
#shader fragment

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};


float4 PSmain(VS_OUTPUT input) : SV_TARGET
{
    //float brightness = 1.0 / length(input.pos. - lightpos.xy + float2(0.2f, 0.5f));
    //float3 result = input.color.xyz * brightness;
    //return saturate(float4(lightpos.xy, brightness, 1.0f));
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

cbuffer u_MVP : register(b0)
{
    float4x4 mvp;
}

VS_OUTPUT VSmain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos, 1.0f), mvp);
    output.color = input.color;

    return output;
}

)"