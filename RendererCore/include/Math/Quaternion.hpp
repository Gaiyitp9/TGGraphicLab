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
		Quaternion();
		Quaternion(Scalar x, Scalar y, Scalar z, Scalar w);
		Quaternion(Scalar angle, Vector<Scalar, 3> axis);
		Quaternion(Vector<Scalar, 3> euler);				// 欧拉角环绕顺序为heading-pitch-bank(yaw-pitch-roll)
															// Y-X-Z(Object Space)或Z-X-Y(World Space 或 Parent Space)
		const Scalar& x() const;
		const Scalar& y() const;
		const Scalar& z() const;
		const Scalar& w() const;
		Scalar& x();
		Scalar& y();
		Scalar& z();
		Scalar& w();

		Quaternion normalized() const;

	public:
		Scalar Dot(Quaternion q);

	private:
		Vector<Scalar, 4> elements;
	};

	using Quaternionf = Quaternion<float>;
	using Quaterniond = Quaternion<double>;
}