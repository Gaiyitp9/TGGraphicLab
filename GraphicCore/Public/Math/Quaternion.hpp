/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Math/Core.hpp"

namespace TG::Math
{
	template<typename Scalar>
	class Quaternion : public RotationBase<Scalar, 4>
	{
	public:
		Quaternion() : m_elements{ 0, 0, 0, 1 } {}
		Quaternion(Scalar x, Scalar y, Scalar z, Scalar w) : m_elements{ x, y, z, w } {}
		explicit Quaternion(const Vector<Scalar, 4>& v) : m_elements{ v } {}
		Quaternion(Scalar angle, const Vector<Scalar, 3>& axis)
		{
			axis.Normalize();
			Scalar halfAngle = angle * static_cast<Scalar>(0.5);
			Scalar sinHalfAngle = std::sin(halfAngle);
            m_elements.X() = sinHalfAngle * axis.X();
            m_elements.Y() = sinHalfAngle * axis.Y();
            m_elements.Z() = sinHalfAngle * axis.Z();
            m_elements.W() = std::cos(halfAngle);
		}
		// 注：欧拉角计算四元数或矩阵需要指定旋转顺序和参考坐标系
		// 当按局部坐标系旋转和按世界坐标系旋转的轴旋转矩阵相乘顺序相反时，这两种旋转等价
		// 欧拉角环绕顺序为heading-pitch-bank(yaw-pitch-roll，局部坐标系)
		// Y-X-Z(Object Space)或Z-X-Y(World Space 或 Parent Space)
		explicit Quaternion(const Vector<Scalar, 3>& euler)
		{
			Scalar halfX = euler.X() * static_cast<Scalar>(0.5);
			Scalar halfY = euler.Y() * static_cast<Scalar>(0.5);
			Scalar halfZ = euler.Z() * static_cast<Scalar>(0.5);
			float sx = std::sin(halfX), cx = std::cos(halfX);
			float sy = std::sin(halfY), cy = std::cos(halfY);
			float sz = std::sin(halfZ), cz = std::cos(halfZ);
            m_elements.X() = sx * cy * cz + cx * sy * sz;
            m_elements.Y() = cx * sy * cz - sx * cy * sz;
            m_elements.Z() = cx * cy * sz - sx * sy * cz;
            m_elements.W() = cx * cy * cz + sx * sy * sz;
		}

		Scalar X() const { return m_elements.X(); }
		Scalar Y() const { return m_elements.Y(); }
		Scalar Z() const { return m_elements.Z(); }
		Scalar W() const { return m_elements.W(); }
		Scalar& X() { return m_elements.X(); }
		Scalar& Y() { return m_elements.Y(); }
		Scalar& Z() { return m_elements.Z(); }
		Scalar& W() { return m_elements.W(); }
		
		Quaternion Conjugate() const { return { -m_elements[0], -m_elements[1], -m_elements[2], m_elements[3] }; }
		Quaternion Normalized() const { return Quaternion(m_elements.Normalized()); }
		void Normalize() { m_elements.Normalize(); }

		Quaternion operator*(const Quaternion& q) const
		{
			float x = m_elements[0] * q.m_elements[3] + m_elements[3] * q.m_elements[0] - m_elements[2] * q.m_elements[1] + m_elements[1] * q.m_elements[2];
			float y = m_elements[1] * q.m_elements[3] + m_elements[2] * q.m_elements[0] + m_elements[3] * q.m_elements[1] - m_elements[0] * q.m_elements[2];
			float z = m_elements[2] * q.m_elements[3] - m_elements[1] * q.m_elements[0] + m_elements[0] * q.m_elements[1] + m_elements[3] * q.m_elements[2];
			float w = m_elements[3] * q.m_elements[3] - m_elements[0] * q.m_elements[0] - m_elements[1] * q.m_elements[1] - m_elements[2] * q.m_elements[2];
			return { x, y, z, w };
		}

		Vector<Scalar, 3> operator*(const Vector<Scalar, 3>& vec) const
		{
			Quaternion q = Normalized();
			Quaternion v{ vec.X(), vec.Y(), vec.Z(), 0 };
			Quaternion v1 = q * v * q.Conjugate();
			return { v1.X(), v1.Y(), v1.Z() };
		}
		Scalar Dot(const Quaternion& q) const { return m_elements.Dot(q.m_elements); }

	private:
		Vector<Scalar, 4> m_elements;
	};

	using QuaternionF = Quaternion<float>;
	using QuaternionD = Quaternion<double>;
}
