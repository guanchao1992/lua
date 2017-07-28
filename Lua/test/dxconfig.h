#pragma once
#include <string>
#include "base\Size.h"
#include "base\Position3D.h"
#include "base\Position2D.h"
#include <D3DX11.h>
#include <DirectXMath.h>

using namespace DirectX;

enum ShaderType
{
	Shader_Normal,
	Shader_Texture,
};

#define Blob_VS_Normal "VS_Normal"
#define Blob_PS_Normal "PS_Normal"
struct SimpleVertexNormal
{
	XMFLOAT4 Pos;  // Position
	XMFLOAT4 Color;  // Color
};

#define Blob_VS_Texture "VS_Texture"
#define Blob_PS_Texture "PS_Texture"
struct SimpleVertexTexture
{
	XMFLOAT4 Pos;  // Position
	XMFLOAT2 Tx0;  // Texture
};


#define Blob_VS_Target "vs_5_0"
#define Blob_PS_Target "ps_5_0"
