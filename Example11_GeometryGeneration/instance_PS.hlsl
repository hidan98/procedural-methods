Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 colour : COLOR;

};

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour = texture0.Sample(Sampler0, input.tex);
	return textureColour;
}