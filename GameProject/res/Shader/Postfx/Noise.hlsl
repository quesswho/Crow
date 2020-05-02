#shader fragment

Texture2D texture0;
SamplerState sampleState;

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer u_Mousepos : register(b0)
{
    float2 c_Mousepos;
}

cbuffer u_Resolution : register(b1)
{
    float2 c_Resolution;
}

float random(float2 xy)
{
    return frac(sin(dot(xy.xy, float2(c_Mousepos.x, c_Mousepos.y))) * 314159);
}

float noise(float2 xy) {
    float2 i = floor(xy);
    float2 f = frac(xy);

    float a = random(i);
    float b = random(i + float2(1.0, 0.0));
    float c = random(i + float2(0.0, 1.0));
    float d = random(i + float2(1.0, 1.0));

    float2 u = f * f * (3.0 - 2.0 * f);

    return lerp(a, b, u.x) +
        (c - a) * u.y * (1.0 - u.x) +
        (d - b) * u.x * u.y;
}

float4 PSmain(VS_OUTPUT input) : SV_TARGET
{
    float result = noise(10.0f * input.pos.xy / c_Resolution);
    return float4(result, result, result, 1.0f);
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