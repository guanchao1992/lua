#pragma once

class Position3D
{
public:
	Position3D()
		:m_x(.0f)
		, m_y(.0f)
		, m_z(.0f)
	{
	}

	Position3D(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	Position3D(const Position3D&pos)
	{
		*this = pos;
	}

	~Position3D()
	{
	}

	inline void setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	inline void setPosition(const Position3D&pos)
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

	inline void setPositionZ(float z)
	{
		m_z = z;
	}

	inline float getPositionX() const
	{
		return m_x;
	}

	inline float getPositionY() const
	{
		return m_y;
	}

	inline float getPositionZ() const
	{
		return m_z;
	}

	inline Position3D operator-() const
	{
		return Position3D(-this->m_x, -this->m_y, -this->m_z);
	}

	inline Position3D& operator=(const Position3D& pos)
	{
		this->m_x = pos.m_x;
		this->m_y = pos.m_y;
		this->m_z = pos.m_z;
		return *this;
	}

	inline Position3D operator+(const Position3D& pos) const
	{
		return Position3D(this->m_x + pos.m_x, this->m_y + pos.m_y, this->m_z + pos.m_z);
	}

	inline Position3D operator-(const Position3D& pos) const
	{
		return Position3D(this->m_x - pos.m_x, this->m_y - pos.m_y, this->m_z - pos.m_z);
	}

	inline bool operator==(const Position3D& pos) const
	{
		return (this->m_x == pos.m_x && this->m_y == pos.m_y&& this->m_z == this->m_z);
	}

private:
	float m_x;
	float m_y;
	float m_z;
};

