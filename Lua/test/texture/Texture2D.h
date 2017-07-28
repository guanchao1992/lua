#pragma once
#include <d3d11.h>
#include <DirectXColors.h>
//#include "..\base\Ref.h"

using namespace DirectX;

class Texture2D 
{
public:
	Texture2D();
	~Texture2D();
	bool init();
	bool createTexture(UINT size, D3D11_USAGE usage, UINT bindFlags, ID3D11Texture2D**outBuffer);

	bool test();
};

