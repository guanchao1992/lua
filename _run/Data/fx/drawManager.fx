//
// Global variable
//
//

#define FLIP_TEXTURE_Y 1

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
	string ResourceName = "E:\\gitproject\\gitLua\\_run\\Data\\image\\1.dds";
	string UIName = "Color Texture";
	string ResourceType = "2D";
>;

SamplerState ColorSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressU = WRAP;
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

void VS_Main(float4 pos : POSITION,
	float4 color : COLOR,
	float2 tx0 : TEXCOORD,
	out float4 outPos : SV_POSITION,
	out float4 outColor : COLOR,
	float2 outTx0 : TEXCOORD)
{
	outPos = pos;
	outColor = color;
	outTx0 = tx0;
}

void PS_Main(float4 pos : POSITION,
	float4 color : COLOR,
	float2 tx0 : TEXCOORD,
	out float4 outColor : SV_TARGET)
{
	float4 txColor = ColorTexture.Sample(ColorSampler, tx0);
	//¼òµ¥µÄ»ìºÏ
	outColor.x = color.x *0.5 + txColor.x*0.5;
	outColor.y = color.y *0.5 + txColor.x*0.5;
	outColor.z = color.z *0.5 + txColor.x*0.5;
	outColor.w = color.w *0.5 + txColor.w*0.5;
}

void VS_Normal(float4 pos : POSITION,
	float4 color : COLOR,
	out float4 outPos : SV_POSITION,
	out float4 outColor : COLOR)
{
	outPos = pos;
	outColor = color;
}

void PS_Normal(float4 pos : SV_POSITION,
	float4 color : COLOR ,
	out float4 outColor: SV_TARGET) 
{  
	outColor = color;
}

void VS_Texture(float4 pos : POSITION,
	float2 tx0 : TEXCOORD,
	out float4 outPos : SV_POSITION,
	out float2 outTx0 : TEXCOORD)
{
	outPos = pos;
	outTx0 = get_corrected_texture_coordinate(tx0);
}

void PS_Texture(float4 pos : SV_POSITION,
	float2 tx0 : TEXCOORD,
	out float4 outColor : SV_TARGET)
{
	outColor = ColorTexture.Sample(ColorSampler, tx0);
}
