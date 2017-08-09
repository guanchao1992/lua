#pragma once

class Position2D
{
public:
	Position2D()
		:m_x(.0f)
		, m_y(.0f)
	{
	}

	Position2D(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	Position2D(const Position2D&pos)
	{
		*this = pos;
	}

	~Position2D()
	{
	}

	inline void setPosition(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	inline void setPosition(const Position2D&pos)
	{
		*this = pos;
	}

	inline void setPositionX(float x)
	{
		m_x = x;
	}

	inline void setPositionY(float y)
	{
		m_y = y;
	}

	inline float getPositionX() const
	{
		return m_x;
	}

	inline float getPositionY() const
	{
		return m_y;
	}

	inline void offsetX(float x)
	{
		m_x += x;
	}

	inline void offsetY(float y)
	{
		m_y += y;
	}

	inline Position2D operator-() const
	{
		return Position2D(-this->m_x, -this->m_y);
	}

	inline Position2D& operator=(const Position2D& pos)
	{
		this->m_x = pos.m_x;
		this->m_y = pos.m_y;
		return *this;
	}

	inline Position2D operator+(const Position2D& pos) const
	{
		return Position2D(this->m_x + pos.m_x, this->m_y + pos.m_y);
	}

	inline Position2D operator-(const Position2D& pos) const
	{
		return Position2D(this->m_x - pos.m_x, this->m_y - pos.m_y);
	}

	inline Position2D operator*(float f) const
	{
		return Position2D(this->m_x *f, this->m_y *f);
	}

	inline bool operator==(const Position2D& pos) const
	{
		return (this->m_x == pos.m_x && this->m_y == pos.m_y);
	}

private:
	float m_x;
	float m_y;
};

