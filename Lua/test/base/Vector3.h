#ifndef ___Vector3__H__
#define ___Vector3__H__

class Vector3
{
public:
	Vector3()
		:m_x(.0f)
		, m_y(.0f)
		, m_z(.0f)
	{
	}

	Vector3(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	Vector3(const Vector3&pos)
	{
		*this = pos;
	}

	~Vector3()
	{
	}

	inline void setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	inline void setPosition(const Vector3&pos)
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

	inline Vector3 operator-() const
	{
		return Vector3(-this->m_x, -this->m_y, -this->m_z);
	}

	inline Vector3& operator=(const Vector3& pos)
	{
		this->m_x = pos.m_x;
		this->m_y = pos.m_y;
		this->m_z = pos.m_z;
		return *this;
	}

	inline Vector3 operator+(const Vector3& pos) const
	{
		return Vector3(this->m_x + pos.m_x, this->m_y + pos.m_y, this->m_z + pos.m_z);
	}

	inline Vector3 operator-(const Vector3& pos) const
	{
		return Vector3(this->m_x - pos.m_x, this->m_y - pos.m_y, this->m_z - pos.m_z);
	}

	inline bool operator==(const Vector3& pos) const
	{
		return (this->m_x == pos.m_x && this->m_y == pos.m_y&& this->m_z == this->m_z);
	}

private:
	float m_x;
	float m_y;
	float m_z;
};

#endif