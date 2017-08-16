#pragma once

class Vector2
{
public:
	Vector2()
		:x(.0f)
		, y(.0f)
	{
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(const Vector2&pos)
	{
		*this = pos;
	}

	~Vector2()
	{
	}

	inline void setValue(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	inline void setValue(const Vector2&pos)
	{
		*this = pos;
	}

	inline void setValueX(float x)
	{
		this->x = x;
	}

	inline void setValueY(float y)
	{
		this->y = y;
	}

	inline float getValueX() const
	{
		return x;
	}

	inline float getValueY() const
	{
		return y;
	}

	inline void offsetX(float x)
	{
		this->x += x;
	}

	inline void offsetY(float y)
	{
		this->y += y;
	}

	inline Vector2 operator-() const
	{
		return Vector2(-this->x, -this->y);
	}

	inline Vector2& operator=(const Vector2& pos)
	{
		this->x = pos.x;
		this->y = pos.y;
		return *this;
	}

	inline Vector2 operator+(const Vector2& pos) const
	{
		return Vector2(this->x + pos.x, this->y + pos.y);
	}

	inline Vector2 operator-(const Vector2& pos) const
	{
		return Vector2(this->x - pos.x, this->y - pos.y);
	}

	inline Vector2 operator*(float f) const
	{
		return Vector2(this->x *f, this->y *f);
	}

	inline bool operator==(const Vector2& pos) const
	{
		return (this->x == pos.x && this->y == pos.y);
	}

public:
	float x;
	float y;
};
