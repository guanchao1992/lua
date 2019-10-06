

// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION;
	float3 color : COLOR;
    float3 origin : ORIGIN;
    float3 scale : SCALE;
	float3 rotate : ROTATEEE;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 origin : ORIGIN;
	float4 color : COLOR;
	float3 p :POSS;
	float l : LEN;
};

struct Pixel
{
	int colour;
};

StructuredBuffer<Pixel> Buffer0 : register(t0);

float3 readPixel(int x, int y)
{
	float3 output;
	uint index = (x + y * 1024);

	output.x = (float)(((Buffer0[index].colour) & 0x000000ff)) / 255.0f;
	output.y = (float)(((Buffer0[index].colour) & 0x0000ff00) >> 8) / 255.0f;
	output.z = (float)(((Buffer0[index].colour) & 0x00ff0000) >> 16) / 255.0f;

	return output;
}


VS_OUTPUT VS_Main( VS_INPUT vx ) 
{
	float3 pv = float3(
		(vx.pos.x - vx.origin.x ),
		(vx.pos.y - vx.origin.y ),
		(vx.pos.z - vx.origin.z));

	//x*cosA - y*sinA  x*sinA + y*cosA
	
	float l = length(pv);

	float3 pv0 = normalize(pv);
	float3 pv1;
	float3 pv2;
	float3 pv3;
	pv1.x = pv0.x * cos(vx.rotate.z) - pv0.y * sin(vx.rotate.z);
	pv1.y = pv0.x * sin(vx.rotate.z) + pv0.y * cos(vx.rotate.z);
	pv1.z = pv0.z;

	pv2.x = pv1.x * cos(vx.rotate.y) - pv1.z * sin(vx.rotate.y);
	pv2.z = pv1.x * sin(vx.rotate.y) + pv1.z * cos(vx.rotate.y);
	pv2.y = pv1.y;

	pv3.y = pv2.y * cos(vx.rotate.x) - pv2.z * sin(vx.rotate.x);
	pv3.z = pv2.y * sin(vx.rotate.x) + pv2.z * cos(vx.rotate.x);
	pv3.x = pv2.x;

	pv = pv3 * l;
	VS_OUTPUT output;
	output.pos = float4(
		vx.origin.x + pv.x,
		vx.origin.y + pv.y,
		vx.origin.z + pv.z,
		1.0f);

	output.pos = mul(output.pos, model);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.origin = float4(vx.origin, 0);
	output.color = float4(vx.color, 1);
	output.l = l;
	output.p = vx.pos - vx.origin;

	return output;
}  

struct PS_INPUT
{
	float3 pos : POSITION;
	float3 color:COLOR;
	float3 origin : ORIGIN;
	float3 scale : SCALE;
	float3 rotate : ROTATEEE;
};

struct PS_OUTPUT
{
	float3 target : SV_TARGET;
};

float4 PS_Main(VS_OUTPUT vso) : SV_TARGET
{
	return vso.color;
}


struct FS_INPUT
{

};

struct FS_OUTPUT
{

};

void FS_Main()
{

}
