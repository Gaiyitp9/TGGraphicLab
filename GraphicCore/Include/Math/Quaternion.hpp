/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Core.hpp"

namespace TG::Math
{
	template<typename Scalar>
	class Quaternion
	{
	public:
		Quaternion() { m_elements[3] = static_cast<Scalar>(1.0); }
		Quaternion(Scalar x, Scalar y, Scalar z, Scalar w)
		{
            m_elements[0] = x;
            m_elements[1] = y;
            m_elements[2] = z;
            m_elements[3] = w;
		}
		explicit Quaternion(const Vector<Scalar, 4>& v)
		{
            m_elements[0] = v[0];
            m_elements[1] = v[1];
            m_elements[2] = v[2];
            m_elements[3] = v[3];
		}
		Quaternion(Scalar angle, Vector<Scalar, 3> axis)
		{
			axis.Normalize();
			Scalar halfAngle = angle * static_cast<Scalar>(0.5);
			Scalar sinHalfAngle = std::sin(halfAngle);
            m_elements[0] = sinHalfAngle * axis.X();
            m_elements[1] = sinHalfAngle * axis.Y();
            m_elements[2] = sinHalfAngle * axis.Z();
            m_elements[3] = std::cos(halfAngle);
		}
		// 欧拉角环绕顺序为heading-pitch-bank(yaw-pitch-roll)
		// Y-X-Z(Object Space)或Z-X-Y(World Space 或 Parent Space)
		explicit Quaternion(Vector<Scalar, 3> euler)
		{
			Scalar halfX = euler.x() * static_cast<Scalar>(0.5);
			Scalar halfY = euler.y() * static_cast<Scalar>(0.5);
			Scalar halfZ = euler.z() * static_cast<Scalar>(0.5);
			float sx = std::sin(halfX), cx = std::cos(halfX);
			float sy = std::sin(halfY), cy = std::cos(halfY);
			float sz = std::sin(halfZ), cz = std::cos(halfZ);
            m_elements[0] = sx * cy * cz + cx * sy * sz;
            m_elements[1] = cx * sy * cz - sx * cy * sz;
            m_elements[2] = cx * cy * sz - sx * sy * cz;
            m_elements[3] = cx * cy * cz + sx * sy * sz;
		}

		const Scalar& X() const { return m_elements[0]; }
		const Scalar& Y() const { return m_elements[1]; }
		const Scalar& Z() const { return m_elements[2]; }
		const Scalar& W() const { return m_elements[3]; }
		Scalar& X() { return m_elements[0]; }
		Scalar& Y() { return m_elements[1]; }
		Scalar& Z() { return m_elements[2]; }
		Scalar& W() { return m_elements[3]; }
		
		Quaternion Conjugate() const { return { -m_elements[0], -m_elements[1], -m_elements[2], m_elements[3] }; }
		Quaternion Normalized() const { return Quaternion(m_elements.Normalized()); }

		Quaternion operator*(Quaternion q) const
		{
			float x = m_elements[0] * q.m_elements[3] + m_elements[3] * q.m_elements[0] - m_elements[2] * q.m_elements[1] + m_elements[1] * q.m_elements[2];
			float y = m_elements[1] * q.m_elements[3] + m_elements[2] * q.m_elements[0] + m_elements[3] * q.m_elements[1] - m_elements[0] * q.m_elements[2];
			float z = m_elements[2] * q.m_elements[3] - m_elements[1] * q.m_elements[0] + m_elements[0] * q.m_elements[1] + m_elements[3] * q.m_elements[2];
			float w = m_elements[3] * q.m_elements[3] - m_elements[0] * q.m_elements[0] - m_elements[1] * q.m_elements[1] - m_elements[2] * q.m_elements[2];
			return { x, y, z, w };
		}

		Vector<Scalar, 3> operator*(Vector<Scalar, 3> vec) const
		{
			Quaternion q = Normalized();
			Quaternion v(vec.x(), vec.y(), vec.z(), 0);
			Quaternion v1 = q * v * q.Conjugate();
			return { v1.X(), v1.Y(), v1.Z() };
		}
		Scalar Dot(Quaternion q) const { return m_elements.Dot(q.m_elements); }

	private:
		Vector<Scalar, 4> m_elements;
	};

	using QuaternionF = Quaternion<float>;
	using QuaternionD = Quaternion<double>;
}
