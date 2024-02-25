#pragma once

#include <iostream>
#include "rtweekend.h"

class Vec3
{
public:
    Vec3() : m_e{0, 0, 0} {}
    Vec3(double e0, double e1, double e2) : m_e{e0, e1, e2} {}

    [[nodiscard]] double X() const { return m_e[0]; }
    [[nodiscard]] double Y() const { return m_e[1]; }
    [[nodiscard]] double Z() const { return m_e[2]; }
    [[nodiscard]] Vec3 Normalized() const
    {
        double length = Length();
        return {m_e[0] / length, m_e[1] / length, m_e[2] / length};
    }

    static Vec3 Random()
    {
        return { RandomDouble(), RandomDouble(), RandomDouble() };
    }

    static Vec3 Random(double min, double max)
    {
        return { RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max) };
    }

    Vec3 operator-() const { return {-m_e[0], -m_e[1], -m_e[2]}; }
    double operator[](int i) const { return m_e[i]; }
    double& operator[](int i) { return m_e[i]; }

    Vec3& operator+=(const Vec3& v)
    {
        m_e[0] += v.m_e[0];
        m_e[1] += v.m_e[1];
        m_e[2] += v.m_e[2];
        return *this;
    }

    Vec3& operator*=(double t)
    {
        m_e[0] *= t;
        m_e[1] *= t;
        m_e[2] *= t;
        return *this;
    }

    Vec3& operator/=(double t)
    {
        return *this *= 1/t;
    }

    [[nodiscard]] double Length() const
    {
        return std::sqrt(LengthSquared());
    }

    [[nodiscard]] double LengthSquared() const
    {
        return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2];
    }

private:
    double m_e[3];
};

using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.X() << ' ' << v.Y() << ' ' << v.Z();
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
    return { u.X() + v.X(), u.Y() + v.Y(), u.Z() + v.Z() };
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
    return { u.X() - v.X(), u.Y() - v.Y(), u.Z() - v.Z() };
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
    return { u.X() * v.X(), u.Y() * v.Y(), u.Z() * v.Z() };
}

inline Vec3 operator*(double t, const Vec3& v)
{
    return { t * v.X(), t * v.Y(), t * v.Z() };
}

inline Vec3 operator*(const Vec3& v, double t)
{
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
    return 1 / t * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
    return u.X() * v.X() + u.Y() * v.Y() + u.Z() * v.Z();
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
    return {
        u.Y() * v.Z() - u.Z() * v.Y(),
        u.Z() * v.X() - u.X() * v.Z(),
        u.X() * v.Y() - u.Y() * v.X()
    };
}

inline Vec3 RandomInUnitSphere()
{
    while (true)
    {
        Vec3 p = Vec3::Random(-1, 1);
        if (p.LengthSquared() < 1)
            return p;
    }
}

inline Vec3 RandomUnitVector()
{
    return RandomInUnitSphere().Normalized();
}

inline Vec3 RandomOnHemisphere(const Vec3& normal)
{
    // rejection method
//    Vec3 sample = RandomUnitVector();

    // Uniform sampling sphere, 根据概率分布函数生成采样
    double r1 = RandomDouble();
    double r2 = RandomDouble();
    double phi = 2 * std::numbers::pi * r2;
    double sinTheta = 2 * std::sqrt(r1 - r1 * r1);
    Vec3 sample{ sinTheta * std::cos(phi), sinTheta * std::sin(phi), 1 - 2 * r1 };

    // 把采样向量变换到半球内
    if (Dot(sample, normal) < 0)
        return -sample;
    else
        return sample;
}
