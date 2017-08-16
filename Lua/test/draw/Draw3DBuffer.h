#pragma once

#include "DrawBuffer.h"


struct DrawCubeBuffer : public DrawBuffer
{
	DrawCubeBuffer(const Vector3& size,const Color4F& color);
	~DrawCubeBuffer();

	virtual void render() override;
	virtual bool bindIndex() override;
	void setShowBoundary(bool setting);
protected:
	bool m_showBoundary;
};


