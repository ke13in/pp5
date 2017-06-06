cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

struct VS_Input
{
	float3 pos : POSITION;
	float3 color : COLOR;
};

struct PS_Input
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

PS_Input main(VS_Input vs)
{
	PS_Input ps;

	float4 pos = float4(vs.pos, 1.0f);
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	ps.color = vs.color;
	ps.pos = pos;


	return ps;

}