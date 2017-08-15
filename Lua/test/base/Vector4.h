#pragma once
#include "baseMath.h"

class Vector4
{
public:
	Vector4()
		: x(.0f)
		, y(.0f)
		, z(.0f)
		, w(.0f)
	{
	}

	Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4(const Vector4&pos)
	{
		*this = pos;
	}

	~Vector4()
	{
	}

	inline bool isZero(float fEpsilon = 0.f) const
	{
		if (fEpsilon == 0.f)
		{
			return x == 0.f && y == 0.f && z == 0.f;
		}
		else
		{
			return MyMath::isZero(x, fEpsilon) && MyMath::isZero(y, fEpsilon) && MyMath::isZero(z, fEpsilon);
		}
	}

	//Ê§°Ü·µ»Øfalse
	inline bool normalize(float fEpsilon = 0.f)
	{
		if (isZero(fEpsilon))
		{
			return false;
		}
		*this = (*this)* (1.0f / getLength());
		return true;
	}

	inline float getLengthSquared() const
	{
		return (x*x + y*y + z*z);
	}

	inline float getLength() const
	{
		return sqrtf(getLengthSquared());
	}

	inline bool setLength(float f, float fEpsilon = 0.f)
	{
		bool ret = normalize(fEpsilon);
		*this = *this * (f);
		return ret;
	}


	inline Vector4 operator-() const
	{
		return Vector4(-x, -y, -z, -w);
	}

	inline Vector4& operator=(const Vector4& pos)
	{
		x = pos.x;
		y = pos.y;
		z = pos.z;
		return *this;
	}

	inline Vector4 operator+(const Vector4& pos) const
	{
		return Vector4(x + pos.x, y + pos.y, z + pos.z, w + pos.w);
	}

	inline Vector4 operator*(float f) const
	{
		return Vector4(x*f, y*f, z*f, w*f);
	}

	inline Vector4 operator-(const Vector4& pos) const
	{
		return Vector4(x - pos.x, y - pos.y, z - pos.z, w - pos.w);
	}

	inline bool operator==(const Vector4& pos) const
	{
		return (x == pos.x && y == pos.y&& z == pos.z && w == pos.w);
	}

	inline float operator[](int i) const
	{
		return this->data[i];
	}

public:
	union
	{
		struct { float x; float y; float z; float w; };
		float data[4];

	};
};

