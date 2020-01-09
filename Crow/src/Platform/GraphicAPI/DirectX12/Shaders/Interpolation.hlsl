R"(
#shader fragment

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

float4 PSmain(VS_OUTPUT input) : SV_TARGET
{
	return input.Color;
}


#shader vertex

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VS_OUTPUT VSmain(float4 inPos : POSITION)
{
	VS_OUTPUT output;

	output.Pos = inPos;
	output.Color = inPos;

	return output;
}
)"