#pragma once
#include "Math.h"
#include "Vector3.h"
#include <corecrt_memcpy_s.h>

class Matrix3
{
public:
	Matrix3()
	{
		setIdentity();
	}

	Matrix3(
		float f00, float f01, float f02,
		float f10, float f11, float f12,
		float f20, float f21, float f22
	)
	{
		m_Column[0][0] = f00; m_Column[1][0] = f10; m_Column[2][0] = f20;
		m_Column[0][1] = f01; m_Column[1][1] = f11; m_Column[2][1] = f21;
		m_Column[0][2] = f02; m_Column[1][2] = f12; m_Column[2][2] = f22;
	}

	Matrix3(const Matrix3&mat)
	{
		*this = mat;
	}

	~Matrix3()
	{
	}

	inline void setIdentity()
	{
		m_Column[0][0] = 1; m_Column[1][0] = 0; m_Column[2][0] = 0;
		m_Column[0][1] = 0; m_Column[1][1] = 1; m_Column[2][1] = 0;
		m_Column[0][2] = 0; m_Column[1][2] = 0; m_Column[2][2] = 1;
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
				MyMath::isFloatEqual(m_Column[0][0], 1, fEpsilon) && MyMath::isFloatEqual(m_Column[1][0], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[2][0], 0, fEpsilon) &&
				MyMath::isFloatEqual(m_Column[0][1], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[1][1], 1, fEpsilon) && MyMath::isFloatEqual(m_Column[2][1], 0, fEpsilon) &&
				MyMath::isFloatEqual(m_Column[0][2], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[1][2], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[2][2], 1, fEpsilon);
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

	inline void set(const float*pData)
	{
		set(pData[0], pData[1], pData[2],
			pData[3], pData[4], pData[5],
			pData[6], pData[7], pData[8]);
	}

	inline void get(float* pData) const
	{
		for (int i = 0; i < 3; ++i)
		{
			pData[i * 3 + 0] = m_Column[0][i];
			pData[i * 3 + 1] = m_Column[1][i];
			pData[i * 3 + 2] = m_Column[2][i];
		}
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

	inline Matrix3 operator-() const
	{
		return Matrix3(
			-m_Column[0][0], -m_Column[1][0], -m_Column[2][0],
			-m_Column[0][1], -m_Column[1][1], -m_Column[2][1],
			-m_Column[0][2], -m_Column[1][2], -m_Column[2][2]);
	}

	inline Matrix3& operator=(const Matrix3& mat)
	{
		memcpy_s(this, sizeof(Matrix3), &mat, sizeof(Matrix3));
		return *this;
	}

	inline Matrix3 operator+(const Matrix3& mat) const
	{
		return Matrix3(
			this->m_Column[0][0] + mat.m_Column[0][0], this->m_Column[1][0] + mat.m_Column[1][0], this->m_Column[2][0] + mat.m_Column[2][0],
			this->m_Column[0][1] + mat.m_Column[0][1], this->m_Column[1][1] + mat.m_Column[1][1], this->m_Column[2][1] + mat.m_Column[2][1],
			this->m_Column[0][2] + mat.m_Column[0][2], this->m_Column[1][2] + mat.m_Column[1][2], this->m_Column[2][2] + mat.m_Column[2][2]);
	}

	inline Matrix3 operator*(float f) const
	{
		return Matrix3(
			this->m_Column[0][0] * f, this->m_Column[1][0] * f, this->m_Column[2][0] * f,
			this->m_Column[0][1] * f, this->m_Column[1][1] * f, this->m_Column[2][1] * f,
			this->m_Column[0][2] * f, this->m_Column[1][2] * f, this->m_Column[2][2] * f);
	}

	inline Matrix3 operator-(const Matrix3& mat) const
	{
		return Matrix3(
			this->m_Column[0] - mat.m_Column[0], this->m_Column[1] - mat.m_Column[1], this->m_Column[2] - mat.m_Column[2],
			this->m_Column[0] - mat.m_Column[0], this->m_Column[1] - mat.m_Column[1], this->m_Column[2] - mat.m_Column[2],
			this->m_Column[0] - mat.m_Column[0], this->m_Column[1] - mat.m_Column[1], this->m_Column[2] - mat.m_Column[2]);
	}

	inline bool operator==(const Matrix3& mat) const
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
	};
};

