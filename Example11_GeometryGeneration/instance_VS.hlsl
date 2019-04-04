
struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 instancePosition : TEXCOORD1;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

};

OutputType main(InputType input)
{
	OutputType output;
	float4 pos = input.position;
	pos.w = 1.0f;

	pos.x += input.instancePosition.x;
	pos.y += input.instancePosition.y;
	pos.z += input.instancePosition.z;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(pos, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	return output;
}