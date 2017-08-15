#pragma once
#include "baseMath.h"
#include <corecrt_math.h>

class Vector3
{
public:
	Vector3()
		:x(.0f)
		, y(.0f)
		, z(.0f)
	{
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3(const Vector3&pos)
	{
		*this = pos;
	}

	~Vector3()
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


	inline Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	inline Vector3& operator=(const Vector3& pos)
	{
		this->x = pos.x;
		this->y = pos.y;
		this->z = pos.z;
		return *this;
	}

	inline Vector3 operator+(const Vector3& pos) const
	{
		return Vector3(x + pos.x, y + pos.y, z + pos.z);
	}

	inline Vector3 operator*(float f) const
	{
		return Vector3(x *f, y *f, z *f);
	}

	inline Vector3 operator-(const Vector3& pos) const
	{
		return Vector3(x - pos.x, y - pos.y, z - pos.z);
	}

	inline bool operator==(const Vector3& pos) const
	{
		return (x == pos.x && y == pos.y&& z == pos.z);
	}

	inline float operator[](int i) const
	{
		return this->data[i];
	}

public:
	union
	{
		struct { float x; float y; float z; };
		float data[3];

	};
};

