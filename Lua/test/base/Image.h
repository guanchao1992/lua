#pragma once
#include <string>
#include "Position2D.h"
#include "Rect2D.h"
#include "TextureBuffer.h"
#include "Ref.h"


//使用引用计数,但不放在自动管理
class Image : public Ref
{
	friend class TextureBuffer;
private:
	//不要传空指针
	Image(const std::string& imageName, TextureBuffer * texture, const Rect2D&rect)
		: m_imageName(imageName)
		, m_textureBuffer(texture)
		, m_rect(rect)
	{
		m_textureBuffer->retain();
	}

public:
	~Image()
	{
		m_textureBuffer->release();
	}

public:
	const Rect2D&			getRect() { return m_rect; };
	const std::string&		getName() { return m_imageName; };
	TextureBuffer*			getTexture() { return m_textureBuffer; };
private:
	std::string			m_imageName;		//图片名
	TextureBuffer*		m_textureBuffer;	//图片指向的纹理
	Rect2D				m_rect;				//图片在纹理上的位置和大小
};

