#pragma once
#include "Vector2.h"
#include "Size.h"

class Rect2D
{
public:

	Rect2D()
		:m_origin(0.f, 0.f)
		, m_size(0.f, 0.f)
	{
	}

	Rect2D(float ValueX, float ValueY, float width, float height)
		: m_origin(ValueX, ValueY)
		, m_size(width, height)
	{
	}

	Rect2D(const Vector2&Value, const Size&size)
		: m_origin(Value)
		, m_size(size)
	{
	}

	Rect2D(const Rect2D&Rect2D)
	{
		*this = Rect2D;
	}

	~Rect2D()
	{
	}

	inline void setRect(float ValueX, float ValueY, float width, float height)
	{
		m_origin.setValue(ValueX, ValueY);
		m_size.setSize(width, height);
	}

	inline void setRect(const Vector2&Value, const Size&size)
	{
		m_origin.setValue(Value);
		m_size.setSize(size);
	}

	inline void setRect(const Rect2D&Rect2D)
	{
		*this = Rect2D;
	}

	inline void setOrigin(const Vector2&Value)
	{
		m_origin.setValue(Value);
	}

	inline void setSize(const Size&size)
	{
		m_size.setSize(size);
	}

	inline const Vector2&getOrigin() const
	{
		return m_origin;
	}

	inline const Size&getSize()	const
	{
		return m_size;
	}

	inline const float getOriginX() const
	{
		return m_origin.getValueX();
	}

	inline const float getOriginY() const
	{
		return m_origin.getValueY();
	}

	inline float getWidth() const
	{
		return m_size.getWidth();
	}

	inline float getHeight() const
	{
		return m_size.getHeight();
	}

	inline Rect2D& operator=(const Rect2D& rect)
	{
		this->m_origin = rect.m_origin;
		this->m_size = rect.m_size;
		return *this;
	}

	inline bool operator==(const Rect2D& rect) const
	{
		return (this->m_origin == rect.m_origin && this->m_size == rect.m_size);
	}
private:
	Vector2	m_origin;
	Size		m_size;
};

