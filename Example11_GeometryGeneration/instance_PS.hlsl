Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 colour : COLOR;

};
//positons of all points to check for voronoi
cbuffer positonInput : register(b0)
{
	float2 position[16];
};
//not really used
cbuffer timeInput : register(b1)
{
	float time;
	float3 padding;
}

float4 main(InputType input) : SV_TARGET
{
	float dis_ = 1;
	float3  colour = float3(0,0,0);
	//loopo through all points
	for (int i = 0; i < 16; i++)
	{
		//calculate the distance between the input position and the current texture pos
		float temp = distance(input.tex, position[i]);
		//get the smallest distance of the two
		dis_ = min(temp, dis_);
	}
	//set the colour value to the distacne 
	colour += dis_;
	//retunr the new colour 
	return float4(colour.x, colour.y, colour.z, 1.0);

}