#pragma once
#include <corecrt_math_defines.h>
#include "Vector4.h"
#include "Vector3.h"
#include <corecrt_math.h>
#include "Matrix4.h"

namespace MyMath
{


	Vector4 AngleQuat(Vector3&vecIn)
	{
		Vector4 out;
		out.w = cos(vecIn[0] / 2.0)*cos(vecIn[1] / 2.0)*cos(vecIn[2] / 2.0) + sin(vecIn[0] / 2.0)*sin(vecIn[1] / 2.0)*sin(vecIn[2] / 2.0);
		out.x = sin(vecIn[0] / 2.0)*cos(vecIn[1] / 2.0)*cos(vecIn[2] / 2.0) - cos(vecIn[0] / 2.0)*sin(vecIn[1] / 2.0)*sin(vecIn[2] / 2.0);
		out.y = cos(vecIn[0] / 2.0)*sin(vecIn[1] / 2.0)*cos(vecIn[2] / 2.0) + sin(vecIn[0] / 2.0)*cos(vecIn[1] / 2.0)*sin(vecIn[2] / 2.0);
		out.z = cos(vecIn[0] / 2.0)*cos(vecIn[1] / 2.0)*sin(vecIn[2] / 2.0) - sin(vecIn[0] / 2.0)*sin(vecIn[1] / 2.0)*cos(vecIn[2] / 2.0);
		return out;
	};

	// 四元数转欧拉角
	Vector3 QuatAngle(const Vector4&vecIn)
	{
		Vector3 out;
		out.x = atan2(2 * (vecIn[3] * vecIn[0] + vecIn[1] * vecIn[2]), 1 - 2 * (vecIn[0] * vecIn[0] + vecIn[1] * vecIn[1]));
		out.y = atan(2 * (vecIn[3] * vecIn[1] - vecIn[2] * vecIn[0]));
		out.z = atan2(2 * (vecIn[3] * vecIn[2] + vecIn[0] * vecIn[1]), 1 - 2 * (vecIn[1] * vecIn[1] + vecIn[2] * vecIn[2]));
		return out;
	}

	// 角度转弧度
	Vector3 DegreeRadian(const Vector3&vecIn)
	{
		Vector3 out;
		out.x = vecIn[0] * (M_PI / 180.0);
		out.y = vecIn[1] * (M_PI / 180.0);
		out.z = vecIn[2] * (M_PI / 180.0);
		return out;
	}

	// 弧度转角度
	Vector3 RadianDegree(const Vector3&vecIn)
	{
		Vector3 out;
		out.x = vecIn[0] * (180.0 / M_PI);
		out.y = vecIn[1] * (180.0 / M_PI);
		out.z = vecIn[2] * (180.0 / M_PI);
		return out;
	}

	// 四元数转矩阵
	Matrix4 QuatMatrix(const Vector4&vecIn)
	{
		float xx, xy, xz, xw, yy, yz, yw, zz, zw;

		xx = vecIn[0] * vecIn[0];
		xy = vecIn[0] * vecIn[1];
		xz = vecIn[0] * vecIn[2];
		xw = vecIn[0] * vecIn[3];
		yy = vecIn[1] * vecIn[1];
		yz = vecIn[1] * vecIn[2];
		yw = vecIn[1] * vecIn[3];
		zz = vecIn[2] * vecIn[2];
		zw = vecIn[2] * vecIn[3];

		Matrix4 out(1 - 2 * (yy + zz), 2 * (xy + zw), 2 * (xz - yw), 0,
			2 * (xy - zw), 1 - 2 * (xx + zz), 2 * (yz + xw), 0,
			2 * (xz + yw), 2 * (yz - xw), 1 - 2 * (xx + yy), 0,
			0, 0, 0, 1);
		return out;
	}

	// 矩阵转四元数
	Vector4 MatrixQuat(const Matrix4& vecIn)
	{
		Vector4 out;
		float s;
		if (vecIn[0][0] > vecIn[1][1] && vecIn[0][0] > vecIn[2][2])
		{
			s = 2 * sqrt(1.0 + vecIn[0][0] - vecIn[1][1] - vecIn[2][2]);
			out.x = 0.25*s;
			out.y = (vecIn[1][0] + vecIn[0][1]) / s;
			out.z = (vecIn[0][2] + vecIn[2][0]) / s;
			out.w = (vecIn[2][1] - vecIn[1][2]) / s;
		}
		else if (vecIn[1][1] > vecIn[2][2])
		{
			s = 2 * sqrt(1.0 + vecIn[1][1] - vecIn[0][0] - vecIn[2][2]);
			out.x = (vecIn[1][0] + vecIn[0][1]) / s;
			out.y = 0.25*s;
			out.z = (vecIn[2][1] + vecIn[1][2]) / s;
			out.w = (vecIn[0][2] - vecIn[2][0]) / s;
		}
		else
		{
			s = 2 * sqrt(1.0 + vecIn[2][2] - vecIn[0][0] - vecIn[1][1]);
			out.x = (vecIn[0][2] + vecIn[2][0]) / s;
			out.y = (vecIn[2][1] + vecIn[1][2]) / s;
			out.z = 0.25*s;
			out.w = (vecIn[1][0] - vecIn[0][1]) / s;
		}
		return out;
	}
};

