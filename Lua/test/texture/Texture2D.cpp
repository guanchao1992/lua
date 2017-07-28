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
	D3D11_TEXTURE2D_DESC cubeMapDesc;           //��������  
	cubeMapDesc.Width = 100;
	cubeMapDesc.Height = 120;
	cubeMapDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;        //���ݸ�ʽ�����������һ��  
																//ָ�������󶨽׶Σ�Render Target �� Shader Resource  
	cubeMapDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	cubeMapDesc.ArraySize = size;                  //�����������а���6������  
	cubeMapDesc.Usage = D3D11_USAGE_DEFAULT;
	cubeMapDesc.CPUAccessFlags = 0;
	cubeMapDesc.MipLevels = 0;                  //0����ʾ�������е�mip��  
												//Misc flags: ��һ��ָ������������cube map���ڶ���ָ����ϵͳ�Լ��������е�mip��  
	cubeMapDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	cubeMapDesc.SampleDesc.Count = 1;           //���ﲻʹ�ö��ز��������  
	cubeMapDesc.SampleDesc.Quality = 0;

	//ʹ������������������  
	ID3D11Texture2D *cubeMap(NULL);
	getD3DDevice()->CreateTexture2D(&cubeMapDesc, 0, &cubeMap);


	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = cubeMapDesc.Format;						//��ʽһ��  
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;     //ָ������һ����������  
	rtvDesc.Texture2DArray.ArraySize = 1;                           //ÿ����ͼ���������һ������  
	rtvDesc.Texture2DArray.MipSlice = 0;                            //ÿ����ͼֻʹ����߲��mip��  

	ID3D11RenderTargetView * dynamicRTV[6];
	//���������ͼ  
	for (UINT i = 0; i < 6; ++i)
	{
		//ÿ����ͼʹ�ö�Ӧ����������  
		//����ָ�������������е���ʼ��������Ϊ����ArraySizeָ��Ϊ1����ֻʹ��һ�ţ�  
		//��������͵�������һ��������  
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		getD3DDevice()->CreateRenderTargetView(cubeMap, &rtvDesc, &dynamicRTV[i]);
	}

	ID3D11ShaderResourceView *dynamicSRV;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = cubeMapDesc.Format;                //��ʽһ��  
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;            //ָ�����ｫ������cube map  
	srvDesc.TextureCube.MipLevels = -1;                 //-1 ��ʾʹ�������е�mip�����ж���ʹ�ö��٣�  
	srvDesc.TextureCube.MostDetailedMip = 0;            //ָ���ϸ��mip�㣬0��ʾ�߲�  
														//������ͼ  
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
		LOG_E("��ȡ��������̨����ʧ��")
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
		//DXTRACE_MSG("��ȡ��������̨����ʧ��!");
		return false;
	}
	return true;
}