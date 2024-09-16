//
// Created by UKMeng on 2024/8/17.
//

#pragma once
#include <iostream>

class Vec4f;

class Vec3f
{
public:
    float x, y, z;

    Vec3f(): x(0), y(0), z(0) {};
    Vec3f(float x): x(x), y(x), z(x) {};
    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {};

    Vec3f operator+(const Vec3f& other) const
    {
        return Vec3f(x + other.x, y + other.y, z + other.z);
    }

    Vec3f operator-(const Vec3f& other) const
    {
        return Vec3f(x - other.x, y - other.y, z - other.z);
    }

    Vec3f operator-() const
    {
        return Vec3f(-x, -y, -z);
    }

    Vec3f operator*(const Vec3f& other) const
    {
        return Vec3f(x * other.x, y * other.y, z * other.z);
    }

    Vec3f operator/(const float& scale) const
    {
        return Vec3f(x / scale, y / scale, z / scale);
    }

    Vec3f operator/(const Vec3f& scale) const
    {
        return Vec3f(x / scale.x, y / scale.y, z / scale.z);
    }

    friend Vec3f operator+(const float& value, const Vec3f& vec)
    {
        return Vec3f(value + vec.x, value + vec.y, value + vec.z);
    }

    friend Vec3f operator-(const float& value, const Vec3f& vec)
    {
        return Vec3f(value - vec.x, value - vec.y, value - vec.z);
    }

    friend Vec3f operator*(const float& scale, const Vec3f& vec)
    {
        return Vec3f(vec.x * scale, vec.y * scale, vec.z * scale);
    }

    // return self reference to support chain operator
    // e.g. a += b += c;
    Vec3f& operator+=(const Vec3f& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3f& operator*=(const Vec3f& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vec3f& operator/=(const float& scale)
    {
        x /= scale;
        y /= scale;
        z /= scale;
        return *this;
    }

    float operator[](int index) const
    {
        return index == 0 ? x : index == 1 ? y : z;
    }

    float& operator[](int index)
    {
        return index == 0 ? x : index == 1 ? y : z;
    }

    Vec3f Normalize() const
    {
        // float rootInv = Q_rsqrt(x * x + y * y + z * z);
        float rootInv = 1 / Length();
        return Vec3f(x * rootInv, y * rootInv, z * rootInv);
    }

    Vec3f Cross(const Vec3f& other) const
    {
        return Vec3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    float Dot(const Vec3f& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    float Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    static Vec3f Min(const Vec3f& a, const Vec3f& b);
    static Vec3f Max(const Vec3f& a, const Vec3f& b);


    Vec4f EularAngleToQuaternion();

    void Print() const;
private:

};

class Vec2i
{
public:
    int x, y;
    Vec2i(): x(0), y(0) {};
    Vec2i(int x): x(x), y(x) {};
    Vec2i(int x, int y) : x(x), y(y) {};
    Vec2i(size_t x, size_t y): x(static_cast<int>(x)), y(static_cast<int>(y)) {};
};

class Vec2f
{
public:
    float x, y;
    Vec2f(): x(0), y(0) {};
    Vec2f(float x): x(x), y(x) {};
    Vec2f(float x, float y) : x(x), y(y) {};
    Vec2f(Vec2i v): x(static_cast<float>(v.x)), y(static_cast<float>(v.y)) {};

    Vec2f operator+(const Vec2f& other) const
    {
        return Vec2f(x + other.x, y + other.y);
    }

    Vec2f operator-(const float& num) const
    {
        return Vec2f(x - num, y - num);
    }

    Vec2f operator*(const float& scale) const
    {
        return Vec2f(x * scale, y * scale);
    }

    friend Vec2f operator*(const float& scale, const Vec2f& vec)
    {
        return Vec2f(vec.x * scale, vec.y * scale);
    }

    Vec2f operator/(const Vec2f& other) const
    {
        return Vec2f(x / other.x, y / other.y);
    }
};

class Vec4f
{
public:
    float x, y, z, w;
    Vec4f(): x(0), y(0), z(0), w(0) {};
    Vec4f(float x): x(x), y(x), z(x), w(x) {};
    Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
    Vec4f(Vec2f v, float z, float w): x(v.x), y(v.y), z(z), w(w) {};
    Vec4f(Vec3f v, float w): x(v.x), y(v.y), z(v.z), w(w) {};

    float Dot(const Vec4f& other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    Vec3f xyz() const {
        return {x, y, z};
    }

    operator Vec3f() const {
        if (w == 0.0f) {
            return {x, y, z};
        } else {
            return {x / w, y / w, z / w};
        }
    }
};