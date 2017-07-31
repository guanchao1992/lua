#include "TextureManager.h"
#include "..\base\NodeList.h"
#include <D3DX11tex.h>
#include "..\..\MyTool\tool_log.h"
#include "VideoManager.h"



SingletonClaseCpp(TextureManager);

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}


TextureBuffer* TextureManager::loadTexture(const std::string&fileName)
{
	auto it = m_mapTexture.find(fileName);
	if (it == m_mapTexture.end())
	{
		TextureBuffer * buffer = TextureBuffer::create(fileName);
		if (buffer == nullptr)
		{
			return nullptr;
		}
		m_mapTexture[fileName] = buffer;
		buffer->retain();
		return buffer;
	}
	return it->second;
}

void TextureManager::releaseTexture(const std::string&fileName)
{
	auto it = m_mapTexture.find(fileName);
	if (it == m_mapTexture.end())
	{
		LOG_E("释放的纹理不存在：%s", fileName.c_str());
		assert(1);
		return;
	}
	it->second->release();
	m_mapTexture.erase(it);
}

void TextureManager::releaseAllTexture()
{
	for (auto it:m_mapTexture)
	{
		it.second->release();
	}
	m_mapTexture.clear();
}

Image* TextureManager::loadImage(const std::string&name)
{
	return nullptr;
	//MSXML2;
}
