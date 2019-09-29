
struct VS_Input
{
    float3 pos : POSITION;
    float3 origin : ORIGIN;
    float3 scale : SCALE;
	float3 rotate : ROTATEEE;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 target : COLOR;
};

float4 VS_Main( VS_Input vx ) : SV_POSITION
{
	float3 pv = float3(
		(vx.pos.x - vx.origin.x)* vx.scale.x, 
		(vx.pos.y - vx.origin.y)* vx.scale.y, 
		(vx.pos.z - vx.origin.z)* vx.scale.z);

	//x*cosA - y*sinA  x*sinA + y*cosA
	
	float l = length(pv);

	float x = pv.x;
	float y = pv.y;
	float z = pv.z;

	pv.x = pv.x * cos(vx.rotate.z) - pv.y * sin(vx.rotate.z);
	pv.y = pv.x * sin(vx.rotate.z) + pv.y * cos(vx.rotate.z);

	pv.x = pv.x * cos(vx.rotate.y) - pv.z * sin(vx.rotate.y);
	pv.z = pv.x * sin(vx.rotate.y) + pv.z * cos(vx.rotate.y);

	pv.y = pv.y * cos(vx.rotate.x) - pv.z * sin(vx.rotate.x);
	pv.z = pv.y * sin(vx.rotate.x) + pv.z * cos(vx.rotate.x);
	
	pv = normalize(pv);
	pv.x = pv.x *l;
	pv.y = pv.y *l;
	pv.z = pv.z *l;
	float4 pos = float4(
		vx.pos.x + pv.x,
		vx.pos.y + pv.y,
		vx.pos.z + pv.z,
		1.0f);

	return pos;
}  
  
float4 PS_Main(float4 pos : SV_POSITION) : SV_TARGET
{  
    return float4( 0.5f, 0.0f, 0.2f, 1.0f );  
} 

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}