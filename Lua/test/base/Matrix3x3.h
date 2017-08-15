#pragma once
#include "Math.h"
#include <winnt.h>
#include <bcrypt.h>
#include "Vector3.h"

class Matrix3x3
{
public:
	Matrix3x3()
	{
		setIdentity();
	}

	Matrix3x3(
		float f00, float f10, float f20,
		float f01, float f11, float f21,
		float f02, float f12, float f22
	)
	{
		this->f00 = f00; this->f10 = f10; this->f20 = f20;
		this->f01 = f01; this->f11 = f11; this->f21 = f21;
		this->f02 = f02; this->f12 = f12; this->f22 = f22;
	}

	Matrix3x3(const Matrix3x3&mat)
	{
		*this = mat;
	}

	~Matrix3x3()
	{
	}

	inline void setIdentity()
	{
		m_Column[0][0] = 1; m_Column[1][0] = 0; m_Column[2][0] = 0;
		m_Column[0][1] = 0; m_Column[1][1] = 1; m_Column[2][1] = 0;
		m_Column[0][2] = 0; m_Column[1][2] = 0; m_Column[2][2] = 2;
	}

	inline void setZero()
	{
		m_Column[0][0] = 0; m_Column[1][0] = 0; m_Column[2][0] = 0;
		m_Column[0][1] = 0; m_Column[1][1] = 0; m_Column[2][1] = 0;
		m_Column[0][2] = 0; m_Column[1][2] = 0; m_Column[2][2] = 0;
	}
	
	inline bool isIdentity(float fEpsilon = 0.f)
	{
		if (fEpsilon == 0.f)
		{
			return 
				(m_Column[0][0] == 1) && (m_Column[1][0] == 0) && (m_Column[2][0] == 0) &&
				(m_Column[0][1] == 0) && (m_Column[1][1] == 1) && (m_Column[2][1] == 0) &&
				(m_Column[0][2] == 0) && (m_Column[1][2] == 0) && (m_Column[2][2] == 1);
		}
		else
		{
			return
				Math::isFloatEqual(m_Column[0][0], 1, fEpsilon) && Math::isFloatEqual(m_Column[1][0], 0, fEpsilon) && Math::isFloatEqual(m_Column[2][0], 0, fEpsilon) &&
				Math::isFloatEqual(m_Column[0][1], 0, fEpsilon) && Math::isFloatEqual(m_Column[1][1], 1, fEpsilon) && Math::isFloatEqual(m_Column[2][1], 0, fEpsilon) &&
				Math::isFloatEqual(m_Column[0][2], 0, fEpsilon) && Math::isFloatEqual(m_Column[1][2], 0, fEpsilon) && Math::isFloatEqual(m_Column[2][2], 1, fEpsilon);
		}
	}

	inline bool isZero(float fEpsilon = 0.f)
	{
		if (fEpsilon == 0.f)
		{
			for (int i = 0; i < 9; ++i)
			{
				if (m_ElementsCM[i] != 0.f)
					return false;
			}
		}
		else
		{
			for (int i = 0; i < 9; ++i)
			{
				if (!Math::isZero(m_ElementsCM[i], fEpsilon))
					return true;
			}

		}
		return true;
	}

	inline void set(
		float f00, float f01, float f02,
		float f10, float f11, float f12,
		float f20, float f21, float f22
	)
	{
		m_Column[0][0] = f00; m_Column[1][0] = f10; m_Column[2][0] = f20;
		m_Column[0][1] = f01; m_Column[1][1] = f11; m_Column[2][1] = f21;
		m_Column[0][2] = f02; m_Column[1][2] = f12; m_Column[2][2] = f22;
	}

	inline void set(IN float*pData)
	{
		memcpy(m_ElementsCM, pData, sizeof(float) * 9);
	}

	inline void get(OUT float* pData)
	{
		memcpy(pData, m_ElementsCM, sizeof(float) * 9);
	}

	inline void setColumn(unsigned int iColumn, const Vector3&values)
	{
		m_Column[iColumn][0] = values[0];
		m_Column[iColumn][1] = values[1];
		m_Column[iColumn][2] = values[2];
	}

	inline void setRow(unsigned int iRow, const Vector3&values)
	{
		m_Column[0][iRow] = values[0];
		m_Column[1][iRow] = values[1];
		m_Column[2][iRow] = values[2];
	}

	inline Vector3 getColumn(unsigned int iColumn)
	{
		return Vector3(m_Column[iColumn][0], m_Column[iColumn][1], m_Column[iColumn][2]);
	}

	inline Vector3 getRow(unsigned int iRow)
	{
		return Vector3(m_Column[0][iRow], m_Column[1][iRow], m_Column[2][iRow]);
	}

	inline float getElement(unsigned int index)
	{
		return m_ElementsCM[index];
	}

	inline void setElement(unsigned int index, float f)
	{
		m_ElementsCM[index] = f;
	}

	inline void setDiagonal(const Vector3&values)
	{
		m_Column[0][0] = values[0];
		m_Column[1][1] = values[1];
		m_Column[2][2] = values[2];
	}

	inline Vector3 getDiagonal()
	{
		return Vector3(m_Column[0][0], m_Column[1][1], m_Column[2][2]);
	}

	inline Vector3 getScalingFactors() const
	{
		Vector3 v;
		v.x = Vector3(m_Column[0][0], m_Column[0][1], m_Column[0][2]).getLength();
		v.y = Vector3(m_Column[1][0], m_Column[1][1], m_Column[1][2]).getLength();
		v.z = Vector3(m_Column[2][0], m_Column[2][1], m_Column[2][2]).getLength();

		return v;
	}

	inline bool setScalingFactors(const Vector3&v, float fEpsilon = 0.f)
	{
		Vector3 tx(m_Column[0][0], m_Column[0][1], m_Column[0][2]);
		Vector3 ty(m_Column[1][0], m_Column[1][1], m_Column[1][2]);
		Vector3 tz(m_Column[2][0], m_Column[2][1], m_Column[2][2]);
		if (!tx.setLength(v.x, fEpsilon) ||
			!ty.setLength(v.y, fEpsilon) ||
			!tz.setLength(v.z, fEpsilon))
		{
			return false;
		}
		m_Column[0][0] = tx.x; m_Column[0][1] = tx.y; m_Column[0][2] = tx.z;
		m_Column[1][0] = ty.x; m_Column[1][1] = ty.y; m_Column[1][2] = ty.z;
		m_Column[2][0] = tz.x; m_Column[2][1] = tz.y; m_Column[2][2] = tz.z;
		return true;
	}

	inline Matrix3x3 operator-() const
	{
		return Matrix3x3(-f00, -f10, -f20, -f01, -f11, -f21, -f02, -f12, -f22);
	}

	inline Matrix3x3& operator=(const Matrix3x3& mat)
	{
		memcpy(this, &mat, sizeof(Matrix3x3));
		return *this;
	}

	inline Matrix3x3 operator+(const Matrix3x3& mat) const
	{
		return Matrix3x3(
			this->f00 + mat.f00, this->f01 + mat.f01, this->f02 + mat.f02,
			this->f10 + mat.f10, this->f11 + mat.f11, this->f12 + mat.f12,
			this->f20 + mat.f20, this->f21 + mat.f21, this->f22 + mat.f22);
	}

	inline Matrix3x3 operator*(float f) const
	{
		return Matrix3x3(
			this->f00 *f, this->f01 *f, this->f02 *f,
			this->f10 *f, this->f11 *f, this->f12 *f,
			this->f20 *f, this->f21 *f, this->f22 *f);
	}

	inline Matrix3x3 operator-(const Matrix3x3& mat) const
	{
		return Matrix3x3(
			this->f00 - mat.f00, this->f01 - mat.f01, this->f02 - mat.f02,
			this->f10 - mat.f10, this->f11 - mat.f11, this->f12 - mat.f12,
			this->f20 - mat.f20, this->f21 - mat.f21, this->f22 - mat.f22);
	}

	inline bool operator==(const Matrix3x3& mat) const
	{
		for (int i=0;i<9;++i)
		{
			if (this->m_ElementsCM[i] != mat.m_ElementsCM[i])
				return false;
		}
		return true;
	}

public:
	union
	{
		float				m_ElementsCM[9];
		float				m_Column[3][3];
		struct 
		{
			float f00, f01, f02, f10, f11, f12, f20, f21, f22;
		};
	};
};

