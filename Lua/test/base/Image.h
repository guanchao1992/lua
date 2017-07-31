#pragma once
#include <string>
#include "Position2D.h"
#include "Rect2D.h"
#include "TextureBuffer.h"


class Image
{
public:
	//��Ҫ����ָ��
	Image(const std::string& imageName, TextureBuffer * texture, const Rect2D&rect)
		: m_imageName(imageName)
		, m_textureBuffer(texture)
		, m_rect(rect)
	{
		m_textureBuffer->retain();
	}

	~Image()
	{
		m_textureBuffer->release();
	}

public:
	const Rect2D&			getRect() { return m_rect; };
	const std::string&		getName() { return m_imageName; };
	TextureBuffer*			getTexture() { return m_textureBuffer; };
private:
	std::string			m_imageName;		//ͼƬ��
	TextureBuffer*		m_textureBuffer;	//ͼƬָ�������
	Rect2D				m_rect;				//ͼƬ�������ϵ�λ�úʹ�С
};

