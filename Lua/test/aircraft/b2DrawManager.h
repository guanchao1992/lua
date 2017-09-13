#pragma once
#include "..\base\Singleton.h"
#include "..\base\Vector2.h"

class DrawLayout;
class DrawNode;

class b2DrawManager
{
	SingletonClase(b2DrawManager);
private:
	b2DrawManager();
public:
	~b2DrawManager();
public:
	void				Init();
	void				Cleanup();

	//每帧绘制前要清空绘制数据
	void				ClearDraw();

	void DrawPolygon(const Vector2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const Vector2* vertices, int32 vertexCount, const b2Color& color);
	void DrawCircle(const Vector2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const Vector2& center, float32 radius, const Vector2& axis, const b2Color& color);
	void DrawSegment(const Vector2& p1, const Vector2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
	void DrawPoint(const Vector2& p, float32 size, const b2Color& color);
private:
	DrawLayout*			m_drawLayout;
	DrawNode*			m_drawNode;
};
