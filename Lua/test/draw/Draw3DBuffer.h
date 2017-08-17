#pragma once

#include "DrawBuffer.h"


struct DrawCubeBuffer : public DrawBuffer
{
	DrawCubeBuffer(const Vector3& size,const Color4F& color);
	~DrawCubeBuffer();

	virtual void render() override;
	virtual bool bindIndex() override;

	//为false时只显示边框
	void setFill(bool setting)
	{
		m_fill = setting;
	}
protected:
	bool m_fill;
};


