/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Math
{
	template<typename Scalar>
	class Quaternion
	{
	public:
		Quaternion() { elements[3] = static_cast<Scalar>(1.0); }
		Quaternion(Scalar x, Scalar y, Scalar z, Scalar w)
		{
			elements[0] = x;
			elements[1] = y;
			elements[2] = z;
			elements[3] = w;
		}
		Quaternion(Vector<Scalar, 4> v)
		{
			elements[0] = v.x();
			elements[1] = v.y();
			elements[2] = v.z();
			elements[3] = v.w();
		}
		Quaternion(Scalar angle, Vector<Scalar, 3> axis)
		{
			axis.Normalize();
			Scalar halfAngle = angle * static_cast<Scalar>(0.5);
			Scalar sinHalfAngle = sin(halfAngle);
			elements[0] = sinHalfAngle * axis.x();
			elements[1] = sinHalfAngle * axis.y();
			elements[2] = sinHalfAngle * axis.z();
			elements[3] = cos(halfAngle);
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
			elements[0] = sx * cy * cz + cx * sy * sz;
			elements[1] = cx * sy * cz - sx * cy * sz;
			elements[2] = cx * cy * sz - sx * sy * cz;
			elements[3] = cx * cy * cz + sx * sy * sz;
		}

		const Scalar& x() const { return elements[0]; }
		const Scalar& y() const { return elements[1]; }
		const Scalar& z() const { return elements[2]; }
		const Scalar& w() const { return elements[3]; }
		Scalar& x() { return elements[0]; }
		Scalar& y() { return elements[1]; }
		Scalar& z() { return elements[2]; }
		Scalar& w() { return elements[3]; }
		
		Quaternion conjugate() const { return Quaternion(-elements[0], -elements[1], -elements[2], elements[3]); }
		Quaternion normalized() const { return Quaternion(elements.normalized()); }

	public:
		Quaternion operator*(Quaternion q) const
		{
			float x = elements[0] * q.elements[3] + elements[3] * q.elements[0] - elements[2] * q.elements[1] + elements[1] * q.elements[2];
			float y = elements[1] * q.elements[3] + elements[2] * q.elements[0] + elements[3] * q.elements[1] - elements[0] * q.elements[2];
			float z = elements[2] * q.elements[3] - elements[1] * q.elements[0] + elements[0] * q.elements[1] + elements[3] * q.elements[2];
			float w = elements[3] * q.elements[3] - elements[0] * q.elements[0] - elements[1] * q.elements[1] - elements[2] * q.elements[2];
			return Quaternion(x, y, z, w);
		}

		Vector<Scalar, 3> operator*(Vector<Scalar, 3> vec) const
		{
			Quaternion q = normalized();
			Quaternion v(vec.x(), vec.y(), vec.z(), 0);
			Quaternion v1 = q * v * q.conjugate();
			return Vector<Scalar, 3>(v1.x(), v1.y(), v1.z());
		}
		Scalar Dot(Quaternion q) const { return elements.Dot(q.elements); }

	private:
		Vector<Scalar, 4> elements;
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}