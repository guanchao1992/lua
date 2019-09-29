float4 VS_Main( float4 pos : POSITION ) : SV_POSITION
{
    return pos;  
}  
  
float4 PS_Main( float4 pos : SV_POSITION ) : SV_TARGET  
{  
	if (pos.x < 450)
    	return float4( pos.x % 255 / 255 * 0.5f, 0.0f, 0.2f, 0.0f );  
	else
    	return float4( 0.0f, 0.0f, 0.4f, 0.0f );  
} 

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}