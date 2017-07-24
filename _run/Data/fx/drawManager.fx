//
// Global variable
//
//

struct SV_OUTPUT
{
	float4 pos : POSITION;
	float4 color : COLOR;
};


void VS_Main(float4 pos : POSITION,
	float4 color : COLOR,
	out float4 outPos : SV_POSITION,
	out float4 outColor : COLOR) 
{
	outPos = pos;// +float4(0.1f, 0.2f, 0.5f, 0.1f);
	outColor = color;// float4(0.1f, 1.0f, 1.0f, 1.0f);
}

void PS_Main(float4 pos : SV_POSITION,
	float4 color : COLOR ,
	out float4 outColor: SV_TARGET) 
{  
	//VS_OUTPUT output = (VS_OUTPUT)0;
	//output.pos = pos;
	//mul(input.position, WVPMatrix);     
	//output.color = input.color;
	//return output;
	outColor = color;
}
