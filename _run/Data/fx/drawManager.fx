//
// Global variable
//
//

#define FLIP_TEXTURE_Y	1
#define ROTE_PI			3.1415926535


cbuffer CBufferPerObject
{
	float4x4 WorldViewProjection : WORLDVIEWPROJECTION<string UIWidget = "None"; >;
}

RasterizerState DisableCulling
{
	CullMode = NONE;
};

Texture2D ColorTexture
<
	string ResourceName = "E:\\gitproject\\gitLua\\_run\\Data\\image\\2.jpg";
	string UIName = "Color Texture";
	string ResourceType = "2D";
>;

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressU = WRAP;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float4 center : CENTER;
	float3 rotate : ROTATE;	
	float3 scale : SCALE;
	float4 color : COLOR;
	float2 tx0 : TEXCOORD;
};

/****************** Utility Functions ********************/
float2 get_corrected_texture_coordinate(float2 textureCoordinate)
{
#if FLIP_TEXTURE_Y
	return float2(textureCoordinate.x, 1.0 - textureCoordinate.y);
#else
	return textureCoordinate;
#endif
}

float4 posToSurePos(float4 pos, float4 center, float3 rotate, float3 scale)
{
	float4 outPos;
	outPos.w = pos.w;
	float4 vecPos = pos - center;
	//旋转
	/*
	float3x4 matrix_x = {	{ 1,0,0 },
							{ 0,cos(rotate.x),sin(rotate.x) },
							{ 0,-sin(rotate.x),cos(rotate.x) },
							{ 0,0,0 }};
*/

	float4x4 matrix_x = {	{ 1,0,0,0},
							{ 0,cos(rotate.x),sin(rotate.x),0 },
							{ 0,-sin(rotate.x),cos(rotate.x),0},
							{ 0,0,0,1} };

	float4x4 matrix_y = {	{ cos(rotate.y),0,sin(rotate.y),0},
							{ 0,1,0,0 },
							{ -sin(rotate.y),0,cos(rotate.y),0},
							{ 0,0,0,1} };

	float4x4 matrix_z = {	{ cos(rotate.z),sin(rotate.z),0,0},
							{ -sin(rotate.z),cos(rotate.z),0,0},
							{ 0,0,1,0 },
							{ 0,0,0,1} };

	/*
	vecPos = mul(vecPos, matrix_x);
	vecPos = mul(vecPos, matrix_y);
	vecPos = mul(vecPos, matrix_z);
*/
	//缩放
	/*
	outPos.x = vecPos.x * scale.x + center.x;
	outPos.y = vecPos.y * scale.y + center.y;
	outPos.z = vecPos.z * scale.z + center.z;
	*/
	outPos = pos;

	return outPos;
}

void VS_Main( VS_INPUT input,
	out float4 outPos : SV_POSITION,
	out float4 outColor : COLOR,
	float2 outTx0 : TEXCOORD)
{
	outPos = input.pos;
	outColor = input.color;
	//outTx0 = input.tx0;
}

void PS_Main(float3 pos : POSITION,
	float4 color : COLOR,
	float2 tx0 : TEXCOORD,
	out float4 outColor : SV_TARGET)
{
	float4 txColor = ColorTexture.Sample(ColorSampler, tx0);
	//简单的混合
	outColor.x = color.x *0.5 + txColor.x*0.5;
	outColor.y = color.y *0.5 + txColor.x*0.5;
	outColor.z = color.z *0.5 + txColor.x*0.5;
	outColor.w = color.w *0.5 + txColor.w*0.5;
}

void VS_Normal(VS_INPUT input,
	out float4 outPos : SV_POSITION,
	out float4 outColor : COLOR)
{
	outPos = posToSurePos(input.pos, input.center, input.rotate, input.scale);
	//outPos = input.pos;
	outColor = input.color;
}

void PS_Normal(float4 pos : SV_POSITION,
	float4 color : COLOR ,
	out float4 outColor: SV_TARGET) 
{  
	outColor = color;
}

void VS_Texture(VS_INPUT input,
	out float4 outPos : SV_POSITION,
	out float2 outTx0 : TEXCOORD)
{
	outPos = input.pos;
	outTx0 = get_corrected_texture_coordinate(input.tx0);
}

void PS_Texture(float4 pos : SV_POSITION,
	float2 tx0 : TEXCOORD,
	out float4 outColor : SV_TARGET)
{
	outColor = ColorTexture.Sample(ColorSampler, tx0);
}
