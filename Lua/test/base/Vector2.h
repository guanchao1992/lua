#ifndef ___Vector2__H__
#define ___Vector2__H__

class Vector2
{
public:
	Vector2()
		:m_x(.0f)
		, m_y(.0f)
	{
	}

	Vector2(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	Vector2(const Vector2&pos)
	{
		*this = pos;
	}

	~Vector2()
	{
	}

	inline void setPosition(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	inline void setPosition(const Vector2&pos)
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

	inline Vector2 operator-() const
	{
		return Vector2(-this->m_x, -this->m_y);
	}

	inline Vector2& operator=(const Vector2& pos)
	{
		this->m_x = pos.m_x;
		this->m_y = pos.m_y;
		return *this;
	}

	inline Vector2 operator+(const Vector2& pos) const
	{
		return Vector2(this->m_x + pos.m_x, this->m_y + pos.m_y);
	}

	inline Vector2 operator-(const Vector2& pos) const
	{
		return Vector2(this->m_x - pos.m_x, this->m_y - pos.m_y);
	}

	inline bool operator==(const Vector2& pos) const
	{
		return (this->m_x == pos.m_x && this->m_y == pos.m_y);
	}

private:
	float m_x;
	float m_y;
};

#endif