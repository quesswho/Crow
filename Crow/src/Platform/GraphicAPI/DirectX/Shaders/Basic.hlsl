R"(
"
#shader fragment

float4 main() : SV_TARGET
{
    return float4(0.0f, 1.0f, 0.0f, 1.0f);
}

#shader vertex

float4 main(float3 position : POSITION) : SV_POSITION
{
    return float4(position, 1.0f);
}
)"