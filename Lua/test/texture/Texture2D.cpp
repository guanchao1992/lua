#include "Texture2D.h"
//#include <D3dx9tex.h>
#include "..\manager\VideoManager.h"
#include <d3d11.h>
#include "..\config.h"
#include <D3DX11tex.h>


Texture2D::Texture2D()
{

}


Texture2D::~Texture2D()
{
}

bool Texture2D::init()
{
	test();
	return true;
}


bool Texture2D::createTexture( UINT size, D3D11_USAGE usage, UINT bindFlags, ID3D11Texture2D**outBuffer)
{
	D3D11_TEXTURE2D_DESC cubeMapDesc;           //纹理描述  
	cubeMapDesc.Width = 100;
	cubeMapDesc.Height = 120;
	cubeMapDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;        //数据格式，跟正常情况一样  
																//指定两个绑定阶段：Render Target 和 Shader Resource  
	cubeMapDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	cubeMapDesc.ArraySize = size;                  //该纹理数组中包含6张纹理  
	cubeMapDesc.Usage = D3D11_USAGE_DEFAULT;
	cubeMapDesc.CPUAccessFlags = 0;
	cubeMapDesc.MipLevels = 0;                  //0，表示产生所有的mip链  
												//Misc flags: 第一个指定该纹理用于cube map，第二个指定让系统自己产生所有的mip链  
	cubeMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	cubeMapDesc.SampleDesc.Count = 1;           //这里不使用多重采样抗锯齿  
	cubeMapDesc.SampleDesc.Quality = 0;

	//使用以上描述创建纹理  
	ID3D11Texture2D *cubeMap(NULL);
	getD3DDevice()->CreateTexture2D(&cubeMapDesc, 0, &cubeMap);


	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = cubeMapDesc.Format;						//格式一样  
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;     //指明这是一个纹理数组  
	rtvDesc.Texture2DArray.ArraySize = 1;                           //每个视图中针对其中一张纹理  
	rtvDesc.Texture2DArray.MipSlice = 0;                            //每个视图只使用最高层的mip链  

	ID3D11RenderTargetView * dynamicRTV[6];
	//逐个创建视图  
	for (UINT i = 0; i < 6; ++i)
	{
		//每个视图使用对应的那张纹理  
		//这里指定了纹理数组中的起始索引，因为上面ArraySize指定为1，即只使用一张，  
		//因此这样就单独锁定一个纹理了  
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		getD3DDevice()->CreateRenderTargetView(cubeMap, &rtvDesc, &dynamicRTV[i]);
	}

	ID3D11ShaderResourceView *dynamicSRV;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = cubeMapDesc.Format;                //格式一样  
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;            //指定这里将它当作cube map  
	srvDesc.TextureCube.MipLevels = -1;                 //-1 表示使用其所有的mip链（有多少使用多少）  
	srvDesc.TextureCube.MostDetailedMip = 0;            //指定最精细的mip层，0表示高层  
														//创建视图  
	getD3DDevice()->CreateShaderResourceView(cubeMap, &srvDesc, &dynamicSRV);

	//D3DX11CreateTextureFromFile();

	return true;
}

bool Texture2D::test()
{
	ID3D11Texture2D* backBufferTexture;
	HRESULT result;
	/*
	// result= getSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

	if (FAILED(result))
	{
		LOG_E("获取交换链后台缓存失败")
		return false;
	}
	*/
	/*
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(VideoManager::getInstance()->getViewSize().getWidth());
	viewport.Height = static_cast<float>(VideoManager::getInstance()->getViewSize().getHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	getD3DContext()->RSSetViewports(1, &viewport);
*/


	//D3DX11_IMAGE_LOAD_INFO imageinfo;

	HRESULT result2 ;
	result = D3DX11CreateTextureFromFile(getD3DDevice(), getAccuratePath("image\\1.jpg").c_str(), NULL, NULL, (ID3D11Resource**)&backBufferTexture, &result2);
	if (FAILED(result))
	{
		//DXTRACE_MSG("获取交换链后台缓存失败!");
		return false;
	}
	return true;
}