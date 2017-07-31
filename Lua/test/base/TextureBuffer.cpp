#include "TextureBuffer.h"
#include "..\config.h"
#include <D3DX11tex.h>
#include "..\manager\VideoManager.h"
#include "..\..\MyTool\tool_log.h"


TextureBuffer* TextureBuffer::create(const std::string&fileName)
{
	TextureBuffer *ret = new TextureBuffer();
	if (ret->init(fileName))
	{
		ret->autorelease();
		return ret;
	}
	return nullptr;
}

bool TextureBuffer::init(const std::string&fileName)
{
	m_fileName = fileName;
	std::string path = getAccuratePath("imageset\\");
	path.append(fileName);
	path.append(".png");
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(getD3DDevice(), path.c_str(), 0, 0, &colorMap_, 0);
	if (FAILED(result))
	{
		assert(1);
		LOG_F_F("纹理文件不纯在:%s", fileName.c_str());
		return false;
	}
	return true;
}
