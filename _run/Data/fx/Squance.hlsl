
struct VS_IN
{
	float3 pos:POSITION;
	float3 color:COLOR;
};
struct VS_OUT
{
	float4 svpos: SV_POSITION;
	float4 color : COLOR;
};

VS_OUT VS_Main(VS_IN vs)
{
	VS_OUT vsout = (VS_OUT)0;
	vsout.svpos = float4(vs.pos, 1);
	vsout.color = float4(vs.color, 1);

    return vsout;  
}  
  
float4 PS_Main(VS_OUT vso) : SV_TARGET
{  
	return vso.color;
} 

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}

