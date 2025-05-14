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
	class Quaternion : public RotationBase<Quaternion<Scalar>, 3>
	{
		using Base = RotationBase<Scalar, 3>;
		using RotationMatrix = Base::RotationMatrix;

	public:
		Quaternion() : m_elements{ 0, 0, 0, 1 } {}
		Quaternion(Scalar x, Scalar y, Scalar z, Scalar w) : m_elements{ x, y, z, w } {}
		explicit Quaternion(const Vector<Scalar, 4>& v) : m_elements{ v } {}
		Quaternion(Scalar angle, const Vector<Scalar, 3>& axis)
		{
			axis.Normalize();
			const Scalar halfAngle = angle * static_cast<Scalar>(0.5);
			const Scalar sinHalfAngle = std::sin(halfAngle);
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
			const Scalar halfX = euler.X() * static_cast<Scalar>(0.5);
			const Scalar halfY = euler.Y() * static_cast<Scalar>(0.5);
			const Scalar halfZ = euler.Z() * static_cast<Scalar>(0.5);
			const float sx = std::sin(halfX), cx = std::cos(halfX);
			const float sy = std::sin(halfY), cy = std::cos(halfY);
			const float sz = std::sin(halfZ), cz = std::cos(halfZ);
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

		RotationMatrix ToRotationMatrix() const
		{
			RotationMatrix result;
			const Scalar tx = 2 * X();
			const Scalar ty = 2 * Y();
			const Scalar tz = 2 * Z();
			const Scalar twx = tx * W();
			const Scalar twy = ty * W();
			const Scalar twz = tz * W();
			const Scalar txx = tx * X();
			const Scalar txy = ty * X();
			const Scalar txz = tz * X();
			const Scalar tyy = ty * Y();
			const Scalar tyz = tz * Y();
			const Scalar tzz = tz * Z();
			result[0, 0] = 1 - (tyy + tzz);
			result[0, 1] = txy - twz;
			result[0, 2] = txz + twy;
			result[1, 0] = txy + twz;
			result[1, 1] = 1 - (txx + tzz);
			result[1, 2] = tyz - twx;
			result[2, 0] = txz - twy;
			result[2, 1] = tyz + twx;
			result[2, 2] = 1 - (txx + tyy);
			return result;
		}
		
		Quaternion Conjugate() const { return { -X(), -Y(), -Z(), W() }; }
		Quaternion Inverse() const { return Vector<Scalar, 4>({ -X(), -Y(), -Z(), W() }) / m_elements.SquaredNorm(); }
		Quaternion Normalized() const { return Quaternion(m_elements.Normalized()); }
		void Normalize() { m_elements.Normalize(); }

		Quaternion operator*(const Quaternion& q) const
		{
			const float x = X() * q.W() + W() * q.X() - Z() * q.Y() + Y() * q.Z();
			const float y = Y() * q.W() + Z() * q.X() + W() * q.Y() - X() * q.Z();
			const float z = Z() * q.W() - Y() * q.X() + X() * q.Y() + W() * q.Z();
			const float w = W() * q.W() - X() * q.X() - Y() * q.Y() - Z() * q.Z();
			return { x, y, z, w };
		}

		Vector<Scalar, 3> operator*(const Vector<Scalar, 3>& vec) const
		{
			const Quaternion q = Normalized();
			const Quaternion v{ vec.X(), vec.Y(), vec.Z(), 0 };
			const Quaternion v1 = q * v * q.Conjugate();
			return { v1.X(), v1.Y(), v1.Z() };
		}
		Scalar Dot(const Quaternion& q) const { return m_elements.Dot(q.m_elements); }

	private:
		Vector<Scalar, 4> m_elements;
	};

	using QuaternionF = Quaternion<float>;
	using QuaternionD = Quaternion<double>;
}
