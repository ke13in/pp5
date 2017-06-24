Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct PS_Input
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

float4 main(PS_Input input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, input.color);
}