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
		Quaternion(Vector<Scalar, 3> euler);				// 欧拉角环绕顺序为heading-pitch-bank(yaw-pitch-roll)
															// Y-X-Z(Object Space)或Z-X-Y(World Space 或 Parent Space)
		const Scalar& x() const { return elements[0]; }
		const Scalar& y() const { return elements[1]; }
		const Scalar& z() const { return elements[2]; }
		const Scalar& w() const { return elements[3]; }
		Scalar& x() { return elements[0]; }
		Scalar& y() { return elements[1]; }
		Scalar& z() { return elements[2]; }
		Scalar& w() { return elements[3]; }

		Quaternion normalized() const { return elements.normalized(); }

	public:
		Scalar Dot(Quaternion q);

	private:
		Vector<Scalar, 4> elements;
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}