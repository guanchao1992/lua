#pragma once
#include <string>
#include "base\Size.h"
#include "base\Vector3.h"
#include "base\Vector2.h"
#include <D3DX11.h>
#include <DirectXMath.h>

using namespace DirectX;

enum ShaderType
{
	Shader_Main,
	Shader_Normal,
	Shader_Texture,
};


#define Blob_VS_Main "VS_Main"
#define Blob_PS_Main "PS_Main"

struct SimpleVertexMain
{
	SimpleVertexMain()
		: Pos(0.f, 0.f, 0.f, 1.0f)
		, Color(1.f, 1.f, 1.f, 1.f)
		, Tx0(0.f, 0.f)
	{ }
	XMFLOAT4 Pos ;		// Position
	XMFLOAT4 Color;		// Color
	XMFLOAT2 Tx0;		// Texture
};

#define Blob_VS_Normal "VS_Normal"
#define Blob_PS_Normal "PS_Normal"

#define Blob_VS_Texture "VS_Texture"
#define Blob_PS_Texture "PS_Texture"

#define Blob_VS_Target "vs_5_0"
#define Blob_PS_Target "ps_5_0"
