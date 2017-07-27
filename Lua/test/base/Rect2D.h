#pragma once
#include "Position2D.h"
#include "Size.h"

class Rect2D
{
public:

	Rect2D()
		:m_origin(0.f, 0.f)
		, m_size(0.f, 0.f)
	{
	}

	Rect2D(float positionX, float positionY, float width, float height)
		: m_origin(positionX, positionY)
		, m_size(width, height)
	{
	}

	Rect2D(const Position2D&position, const Size&size)
		: m_origin(position)
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

	inline void setRect(float positionX, float positionY, float width, float height)
	{
		m_origin.setPosition(positionX, positionY);
		m_size.setSize(width, height);
	}

	inline void setRect(const Position2D&position, const Size&size)
	{
		m_origin.setPosition(position);
		m_size.setSize(size);
	}

	inline void setRect(const Rect2D&Rect2D)
	{
		*this = Rect2D;
	}

	inline void setOrigin(const Position2D&position)
	{
		m_origin.setPosition(position);
	}

	inline void setSize(const Size&size)
	{
		m_size.setSize(size);
	}

	inline const Position2D&getOrigin() const
	{
		return m_origin;
	}

	inline const Size&getSize()	const
	{
		return m_size;
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
	Position2D	m_origin;
	Size		m_size;
};

