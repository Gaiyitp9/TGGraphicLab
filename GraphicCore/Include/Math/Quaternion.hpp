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
		explicit Quaternion(Vector<Scalar, 4> v)
		{
            m_elements[0] = v.x();
            m_elements[1] = v.y();
            m_elements[2] = v.z();
            m_elements[3] = v.w();
		}
		Quaternion(Scalar angle, Vector<Scalar, 3> axis)
		{
			axis.Normalize();
			Scalar halfAngle = angle * static_cast<Scalar>(0.5);
			Scalar sinHalfAngle = sin(halfAngle);
            m_elements[0] = sinHalfAngle * axis.x();
            m_elements[1] = sinHalfAngle * axis.y();
            m_elements[2] = sinHalfAngle * axis.z();
            m_elements[3] = cos(halfAngle);
		}
		// 欧拉角环绕顺序为heading-pitch-bank(yaw-pitch-roll)
		// Y-X-Z(Object Space)或Z-X-Y(World Space 或 Parent Space)
		Quaternion(Vector<Scalar, 3> euler)				
		{
			Scalar halfX = euler.x() * static_cast<Scalar>(0.5);
			Scalar halfY = euler.y() * static_cast<Scalar>(0.5);
			Scalar halfZ = euler.z() * static_cast<Scalar>(0.5);
			float sx = sin(halfX), cx = cos(halfX);
			float sy = sin(halfY), cy = cos(halfY);
			float sz = sin(halfZ), cz = cos(halfZ);
            m_elements[0] = sx * cy * cz + cx * sy * sz;
            m_elements[1] = cx * sy * cz - sx * cy * sz;
            m_elements[2] = cx * cy * sz - sx * sy * cz;
            m_elements[3] = cx * cy * cz + sx * sy * sz;
		}

		const Scalar& x() const { return m_elements[0]; }
		const Scalar& y() const { return m_elements[1]; }
		const Scalar& z() const { return m_elements[2]; }
		const Scalar& w() const { return m_elements[3]; }
		Scalar& x() { return m_elements[0]; }
		Scalar& y() { return m_elements[1]; }
		Scalar& z() { return m_elements[2]; }
		Scalar& w() { return m_elements[3]; }
		
		Quaternion conjugate() const { return Quaternion(-m_elements[0], -m_elements[1], -m_elements[2], m_elements[3]); }
		Quaternion normalized() const { return Quaternion(m_elements.normalized()); }

	public:
		Quaternion operator*(Quaternion q) const
		{
			float x = m_elements[0] * q.m_elements[3] + m_elements[3] * q.m_elements[0] - m_elements[2] * q.m_elements[1] + m_elements[1] * q.m_elements[2];
			float y = m_elements[1] * q.m_elements[3] + m_elements[2] * q.m_elements[0] + m_elements[3] * q.m_elements[1] - m_elements[0] * q.m_elements[2];
			float z = m_elements[2] * q.m_elements[3] - m_elements[1] * q.m_elements[0] + m_elements[0] * q.m_elements[1] + m_elements[3] * q.m_elements[2];
			float w = m_elements[3] * q.m_elements[3] - m_elements[0] * q.m_elements[0] - m_elements[1] * q.m_elements[1] - m_elements[2] * q.m_elements[2];
			return Quaternion(x, y, z, w);
		}

		Vector<Scalar, 3> operator*(Vector<Scalar, 3> vec) const
		{
			Quaternion q = normalized();
			Quaternion v(vec.x(), vec.y(), vec.z(), 0);
			Quaternion v1 = q * v * q.conjugate();
			return Vector<Scalar, 3>(v1.x(), v1.y(), v1.z());
		}
		Scalar Dot(Quaternion q) const { return m_elements.Dot(q.m_elements); }

	private:
		Vector<Scalar, 4> m_elements;
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}
