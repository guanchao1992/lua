#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <corecrt_memcpy_s.h>
#include "baseMath.h"

class Matrix4
{
public:
	Matrix4()
	{
		setIdentity();
	}

	Matrix4(
		float f00, float f10, float f20, float f30,
		float f01, float f11, float f21, float f31,
		float f02, float f12, float f22, float f32,
		float f03, float f13, float f23, float f33
	)
	{
		m_Column[0][0] = f00; m_Column[1][0] = f10; m_Column[2][0] = f20; m_Column[3][0] = f30;
		m_Column[0][1] = f01; m_Column[1][1] = f11; m_Column[2][1] = f21; m_Column[3][1] = f31;
		m_Column[0][2] = f02; m_Column[1][2] = f12; m_Column[2][2] = f22; m_Column[3][2] = f32;
		m_Column[0][3] = f03; m_Column[1][3] = f13; m_Column[2][3] = f23; m_Column[3][3] = f33;
	}

	Matrix4(const Matrix4&mat)
	{
		*this = mat;
	}

	~Matrix4()
	{
	}

	inline void setIdentity()
	{
		m_Column[0][0] = 1; m_Column[1][0] = 0; m_Column[2][0] = 0; m_Column[3][0] = 0;
		m_Column[0][1] = 0; m_Column[1][1] = 1; m_Column[2][1] = 0; m_Column[3][1] = 0;
		m_Column[0][2] = 0; m_Column[1][2] = 0; m_Column[2][2] = 1; m_Column[3][2] = 0;
		m_Column[0][3] = 0; m_Column[1][3] = 0; m_Column[2][3] = 0; m_Column[3][3] = 1;
	}

	inline void setZero()
	{
		m_Column[0][0] = 0; m_Column[1][0] = 0; m_Column[2][0] = 0; m_Column[3][0] = 0;
		m_Column[0][1] = 0; m_Column[1][1] = 0; m_Column[2][1] = 0; m_Column[3][1] = 0;
		m_Column[0][2] = 0; m_Column[1][2] = 0; m_Column[2][2] = 0; m_Column[3][2] = 0;
		m_Column[0][3] = 0; m_Column[1][3] = 0; m_Column[2][3] = 0; m_Column[3][3] = 0;
	}

	inline bool isIdentity(float fEpsilon = 0.f)
	{
		if (fEpsilon == 0.f)
		{
			return
				(m_Column[0][0] == 1) && (m_Column[1][0] == 0) && (m_Column[2][0] == 0) && (m_Column[3][0] == 0) &&
				(m_Column[0][1] == 0) && (m_Column[1][1] == 1) && (m_Column[2][1] == 0) && (m_Column[3][1] == 0) &&
				(m_Column[0][2] == 0) && (m_Column[1][2] == 0) && (m_Column[2][2] == 1) && (m_Column[3][2] == 0) &&
				(m_Column[0][3] == 0) && (m_Column[1][3] == 0) && (m_Column[2][3] == 0) && (m_Column[3][3] == 1);
		}
		else
		{
			return
				MyMath::isFloatEqual(m_Column[0][0], 1, fEpsilon) && MyMath::isFloatEqual(m_Column[1][0], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[2][0], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[3][0], 0, fEpsilon) &&
				MyMath::isFloatEqual(m_Column[0][1], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[1][1], 1, fEpsilon) && MyMath::isFloatEqual(m_Column[2][1], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[3][1], 0, fEpsilon) &&
				MyMath::isFloatEqual(m_Column[0][2], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[1][2], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[2][2], 1, fEpsilon) && MyMath::isFloatEqual(m_Column[3][2], 0, fEpsilon) &&
				MyMath::isFloatEqual(m_Column[0][3], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[1][3], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[2][3], 0, fEpsilon) && MyMath::isFloatEqual(m_Column[3][3], 1, fEpsilon);
		}
	}

	inline bool isZero(float fEpsilon = 0.f)
	{
		if (fEpsilon == 0.f)
		{
			for (int i = 0; i < 16; ++i)
			{
				if (m_ElementsCM[i] != 0.f)
					return false;
			}
		}
		else
		{
			for (int i = 0; i < 16; ++i)
			{
				if (!MyMath::isZero(m_ElementsCM[i], fEpsilon))
					return true;
			}

		}
		return true;
	}

	inline void set(
		float f00, float f10, float f20, float f30,
		float f01, float f11, float f21, float f31,
		float f02, float f12, float f22, float f32,
		float f03, float f13, float f23, float f33
	)
	{
		m_Column[0][0] = f00; m_Column[1][0] = f10; m_Column[2][0] = f20; m_Column[3][0] = f30;
		m_Column[0][1] = f01; m_Column[1][1] = f11; m_Column[2][1] = f21; m_Column[3][1] = f31;
		m_Column[0][2] = f02; m_Column[1][2] = f12; m_Column[2][2] = f22; m_Column[3][2] = f32;
		m_Column[0][3] = f03; m_Column[1][3] = f13; m_Column[2][3] = f23; m_Column[3][3] = f33;
	}

	inline void set(const float*pData)
	{
		set(pData[0], pData[1], pData[2], pData[3], 
			pData[4], pData[5], pData[6], pData[7], 
			pData[8], pData[9], pData[10], pData[11], 
			pData[12], pData[13], pData[14], pData[15]);
	}

	inline void get(float* pData) const
	{
		for (int i = 0; i < 4; ++i)
		{
			pData[i * 4 + 0] = m_Column[0][i];
			pData[i * 4 + 1] = m_Column[1][i];
			pData[i * 4 + 2] = m_Column[2][i];
			pData[i * 4 + 3] = m_Column[3][i];
		}
	}

	inline void setColumn(unsigned int iColumn, const Vector3&values)
	{
		m_Column[iColumn][0] = values[0];
		m_Column[iColumn][1] = values[1];
		m_Column[iColumn][2] = values[2];
	}

	inline void setColumn(unsigned int iColumn, const Vector4&values)
	{
		m_Column[iColumn][0] = values[0];
		m_Column[iColumn][1] = values[1];
		m_Column[iColumn][2] = values[2];
		m_Column[iColumn][3] = values[3];
	}

	inline void setRow(unsigned int iRow, const Vector3&values)
	{
		m_Column[0][iRow] = values[0];
		m_Column[1][iRow] = values[1];
		m_Column[2][iRow] = values[2];
	}

	inline void setRow(unsigned int iRow, const Vector4&values)
	{
		m_Column[0][iRow] = values[0];
		m_Column[1][iRow] = values[1];
		m_Column[2][iRow] = values[2];
		m_Column[3][iRow] = values[3];
	}

	inline Vector4 getColumn(unsigned int iColumn)
	{
		return Vector4(m_Column[iColumn][0], m_Column[iColumn][1], m_Column[iColumn][2], m_Column[iColumn][3]);
	}

	inline Vector4 getRow(unsigned int iRow)
	{
		return Vector4(m_Column[0][iRow], m_Column[1][iRow], m_Column[2][iRow], m_Column[3][iRow]);
	}

	inline float getElement(unsigned int index)
	{
		return m_ElementsCM[index];
	}

	inline void setElement(unsigned int index, float f)
	{
		m_ElementsCM[index] = f;
	}

	inline void setDiagonal(const Vector4&values)
	{
		m_Column[0][0] = values[0];
		m_Column[1][1] = values[1];
		m_Column[2][2] = values[2];
		m_Column[3][3] = values[3];
	}

	inline Vector4 getDiagonal()
	{
		return Vector4(m_Column[0][0], m_Column[1][1], m_Column[2][2], m_Column[2][2]);
	}

	inline Vector3 getScalingFactors() const
	{
		Vector3 v;
		v.x = Vector3(m_Column[0][0], m_Column[0][1], m_Column[0][2]).getLength();
		v.y = Vector3(m_Column[1][0], m_Column[1][1], m_Column[1][2]).getLength();
		v.z = Vector3(m_Column[2][0], m_Column[2][1], m_Column[2][2]).getLength();

		return v;
	}

	inline bool setScalingFactors(const Vector4&v, float fEpsilon = 0.f)
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

	inline Matrix4 operator-() const
	{
		return Matrix4(
			-m_Column[0][0], -m_Column[1][0], -m_Column[2][0], -m_Column[3][0],
			-m_Column[0][1], -m_Column[1][1], -m_Column[2][1], -m_Column[3][1],
			-m_Column[0][2], -m_Column[1][2], -m_Column[2][2], -m_Column[3][2],
			-m_Column[0][3], -m_Column[1][3], -m_Column[2][3], -m_Column[3][3]);
	}

	inline Matrix4& operator=(const Matrix4& mat)
	{
		memcpy_s(this, sizeof(Matrix4), &mat, sizeof(Matrix4));
		return *this;
	}

	inline Matrix4 operator+(const Matrix4& mat) const
	{
		return Matrix4(
			m_Column[0][0] + mat.m_Column[0][0], m_Column[1][0] + mat.m_Column[1][0], m_Column[2][0] + mat.m_Column[2][0], m_Column[3][0] + mat.m_Column[3][0],
			m_Column[0][1] + mat.m_Column[0][1], m_Column[1][1] + mat.m_Column[1][1], m_Column[2][1] + mat.m_Column[2][1], m_Column[3][1] + mat.m_Column[3][1],
			m_Column[0][2] + mat.m_Column[0][2], m_Column[1][2] + mat.m_Column[1][2], m_Column[2][2] + mat.m_Column[2][2], m_Column[3][2] + mat.m_Column[3][2],
			m_Column[0][3] + mat.m_Column[0][3], m_Column[1][3] + mat.m_Column[1][3], m_Column[2][3] + mat.m_Column[2][3], m_Column[3][3] + mat.m_Column[3][3]);
	}

	inline Matrix4 operator*(float f) const
	{
		return Matrix4(
			m_Column[0][0] * f, m_Column[1][0] * f, m_Column[2][0] * f, m_Column[3][0] * f,
			m_Column[0][1] * f, m_Column[1][1] * f, m_Column[2][1] * f, m_Column[3][1] * f,
			m_Column[0][2] * f, m_Column[1][2] * f, m_Column[2][2] * f, m_Column[3][2] * f,
			m_Column[0][3] * f, m_Column[1][3] * f, m_Column[2][3] * f, m_Column[3][3] * f);
	}

	const Matrix4 multiply(const Matrix4&rhs) const
	{
		Matrix4 r;

		for (int i = 0; i < 4; ++i)
		{
			r.m_Column[i][0] = (m_Column[0][0] * rhs.m_Column[i][0]) + (m_Column[1][0] * rhs.m_Column[i][1]) + (m_Column[2][0] * rhs.m_Column[i][2]) + (m_Column[3][0] * rhs.m_Column[i][3]);
			r.m_Column[i][1] = (m_Column[0][1] * rhs.m_Column[i][0]) + (m_Column[1][1] * rhs.m_Column[i][1]) + (m_Column[2][1] * rhs.m_Column[i][2]) + (m_Column[3][1] * rhs.m_Column[i][3]);
			r.m_Column[i][2] = (m_Column[0][2] * rhs.m_Column[i][0]) + (m_Column[1][2] * rhs.m_Column[i][1]) + (m_Column[2][2] * rhs.m_Column[i][2]) + (m_Column[3][2] * rhs.m_Column[i][3]);
			r.m_Column[i][3] = (m_Column[0][3] * rhs.m_Column[i][0]) + (m_Column[1][3] * rhs.m_Column[i][1]) + (m_Column[2][3] * rhs.m_Column[i][2]) + (m_Column[3][3] * rhs.m_Column[i][3]);
		}
		return r;
	}

	inline const Vector4 transform(const Vector4&v) const
	{
		const float rx = m_Column[0][0] * v.x + m_Column[1][0] * v.y + m_Column[2][0] * v.z + m_Column[3][0] * v.w;
		const float ry = m_Column[0][1] * v.x + m_Column[1][1] * v.y + m_Column[2][1] * v.z + m_Column[3][1] * v.w;
		const float rz = m_Column[0][2] * v.x + m_Column[1][2] * v.y + m_Column[2][2] * v.z + m_Column[3][2] * v.w;
		const float rw = m_Column[0][3] * v.x + m_Column[1][3] * v.y + m_Column[2][3] * v.z + m_Column[3][3] * v.w;
		return Vector4(rx, ry, rz, rw);
	}

	inline const Vector3 transformPosition(const Vector3&v) const
	{
		const float rx = m_Column[0][0] * v.x + m_Column[1][0] * v.y + m_Column[2][0] * v.z + m_Column[3][0];
		const float ry = m_Column[0][1] * v.x + m_Column[1][1] * v.y + m_Column[2][1] * v.z + m_Column[3][1];
		const float rz = m_Column[0][2] * v.x + m_Column[1][2] * v.y + m_Column[2][2] * v.z + m_Column[3][2];
		return Vector3(rx, ry, rz);
	}

	inline const Vector3 transformDirection(const Vector3&v) const
	{
		const float rx = m_Column[0][0] * v.x + m_Column[1][0] * v.y + m_Column[2][0] * v.z ;
		const float ry = m_Column[0][1] * v.x + m_Column[1][1] * v.y + m_Column[2][1] * v.z ;
		const float rz = m_Column[0][2] * v.x + m_Column[1][2] * v.y + m_Column[2][2] * v.z ;
		return Vector3(rx, ry, rz);
	}

	inline Matrix4 operator-(const Matrix4& mat) const
	{
		return Matrix4(
			m_Column[0][0] - mat.m_Column[0][0], m_Column[1][0] - mat.m_Column[1][0], m_Column[2][0] - mat.m_Column[2][0], m_Column[3][0] - mat.m_Column[3][0],
			m_Column[0][1] - mat.m_Column[0][1], m_Column[1][1] - mat.m_Column[1][1], m_Column[2][1] - mat.m_Column[2][1], m_Column[3][1] - mat.m_Column[3][1],
			m_Column[0][2] - mat.m_Column[0][2], m_Column[1][2] - mat.m_Column[1][2], m_Column[2][2] - mat.m_Column[2][2], m_Column[3][2] - mat.m_Column[3][2],
			m_Column[0][3] - mat.m_Column[0][3], m_Column[1][3] - mat.m_Column[1][3], m_Column[2][3] - mat.m_Column[2][3], m_Column[3][3] - mat.m_Column[3][3]);
	}

	inline bool operator==(const Matrix4& mat) const
	{
		for (int i = 0; i < 16; ++i)
		{
			if (this->m_ElementsCM[i] != mat.m_ElementsCM[i])
				return false;
		}
		return true;
	}

	inline float* operator[](int i) const
	{
		return (float*)m_Column[i];
	}

public:
	union
	{
		float				m_ElementsCM[16];
		float				m_Column[4][4];
	};
};

