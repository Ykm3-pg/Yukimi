#pragma once

namespace Yukimi
{

namespace Math
{

inline static constexpr float EPSILON = 1e-5f;

inline static bool NearlyEqual(const float a, const float b) noexcept
{
    return std::fabs(a - b) <= EPSILON;
}

}

struct Vector2
{
    float x = 0.f;
    float y = 0.f;

    constexpr Vector2() noexcept : x(0.f), y(0.f) {}
    constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}

    inline float Length() const noexcept
    {
        // feature:hypotf‚Å‚ÌŽÀ‘•‚àŒŸ“¢
        return std::sqrtf(x * x + y * y);
    }
    inline float LengthSquare() const noexcept
    {
        return (x * x + y * y);
    }
    inline Vector2& Normalize() noexcept
    {
        float len = Length();
        if (Math::NearlyEqual(len, 0.f))
        {
            assert(false && "zero length normalization in Vector2::Normalize()");
            *this = Vector2::Zero();
            return *this;
        }

        *this /= len;
        return *this;
    }
    inline bool IsZero() const noexcept
    {
        return Math::NearlyEqual(x, 0.f) && Math::NearlyEqual(y, 0.f);
    }

    inline static float Dot(const Vector2& lhs, const Vector2& rhs) noexcept
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;

    }
    inline static float Cross(const Vector2& lhs, const Vector2& rhs) noexcept
    {
        return lhs.x * rhs.y - lhs.y * rhs.x;

    }
    inline static float Distance(const Vector2& lhs, const Vector2& rhs) noexcept
    {
        Vector2 dist;
        dist = lhs - rhs;
        return dist.Length();
    }

    inline static constexpr Vector2 Zero() noexcept
    {
        return Vector2(0.f, 0.f);
    }
    inline static constexpr Vector2 One() noexcept
    {
        return Vector2(1.f, 1.f);
    }
    inline static constexpr Vector2 Right() noexcept
    {
        return Vector2(1.f, 0.f);
    }
    inline static constexpr Vector2 Left() noexcept
    {
        return Vector2(-1.f, 0.f);
    }
    inline static constexpr Vector2 Up() noexcept
    {
        return Vector2(0.f, 1.f);
    }
    inline static constexpr Vector2 Down() noexcept
    {
        return Vector2(0.f, -1.f);
    }

    inline constexpr Vector2 operator+() const noexcept
    {
        return *this;
    }
    inline constexpr Vector2 operator-() const noexcept
    {
        return Vector2(-x, -y);
    }

    inline constexpr Vector2 operator+(const Vector2& rhs) const noexcept
    {
        return Vector2(x + rhs.x, y + rhs.y);
    }
    inline constexpr Vector2 operator-(const Vector2& rhs) const noexcept
    {
        return Vector2(x - rhs.x, y - rhs.y);
    }
    inline constexpr Vector2 operator*(const float s) const noexcept
    {
        return Vector2(x * s, y * s);
    }
    inline Vector2 operator/(const float s) const noexcept
    {
        if (Math::NearlyEqual(s, 0.f))
        {
            assert(false && "division by zero in Vector2::operator/");
            return Vector2::Zero();
        }
        return Vector2(x / s, y / s);
    }

    inline constexpr Vector2& operator+=(const Vector2& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    inline constexpr Vector2& operator-=(const Vector2& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    inline constexpr Vector2& operator*=(const float s) noexcept
    {
        x *= s;
        y *= s;
        return *this;
    }
    inline Vector2& operator/=(const float s) noexcept
    {
        if (Math::NearlyEqual(s, 0.f))
        {
            assert(false && "division by zero in Vector2::operator/=");
            *this = Vector2::Zero();
            return *this;
        }
        x /= s;
        y /= s;
        return *this;
    }

    inline bool operator==(const Vector2& rhs) const noexcept
    {
        return Math::NearlyEqual(x, rhs.x) && Math::NearlyEqual(y, rhs.y);
    }
    inline bool operator!=(const Vector2& rhs) const noexcept
    {
        return !(*this == rhs);
    }
};
inline constexpr Vector2 operator*(float s, const Vector2& v) noexcept
{
    return Vector2(v.x * s, v.y * s);
}

struct Vector3
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;

    constexpr Vector3() noexcept : x(0.f), y(0.f), z(0.f) {}
    constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

    inline float Length() const noexcept
    {
        // feature:hypotf‚Å‚ÌŽÀ‘•‚àŒŸ“¢
        return std::sqrtf(x * x + y * y + z * z);
    }
    inline float LengthSquare() const noexcept
    {
        return (x * x + y * y + z * z);
    }
    inline Vector3& Normalize() noexcept
    {
        float len = Length();
        if (Math::NearlyEqual(len, 0.f))
        {
            assert(false && "zero length normalization in Vector3::Normalize()");
            *this = Vector3::Zero();
            return *this;
        }

        *this /= len;
        return *this;
    }
    inline bool IsZero() const noexcept
    {
        return Math::NearlyEqual(x, 0.f) && Math::NearlyEqual(y, 0.f) && Math::NearlyEqual(z, 0.f);
    }

    inline static float Dot(const Vector3& lhs, const Vector3& rhs) noexcept
    {
        return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;

    }
    inline static Vector3 Cross(const Vector3& lhs, const Vector3& rhs) noexcept
    {
        float cx = lhs.y * rhs.z - lhs.z * rhs.y;
        float cy = lhs.z * rhs.x - lhs.x * rhs.z;
        float cz = lhs.x * rhs.y - lhs.y * rhs.x;
        return Vector3(cx, cy, cz);
    }
    inline static float Distance(const Vector3& lhs, const Vector3& rhs) noexcept
    {
        Vector3 dist;
        dist = lhs - rhs;
        return dist.Length();
    }

    inline static constexpr Vector3 Zero() noexcept
    {
        return Vector3(0.f, 0.f, 0.f);
    }
    inline static constexpr Vector3 One() noexcept
    {
        return Vector3(1.f, 1.f, 1.f);
    }
    inline static constexpr Vector3 Right() noexcept
    {
        return Vector3(1.f, 0.f, 0.f);
    }
    inline static constexpr Vector3 Left() noexcept
    {
        return Vector3(-1.f, 0.f, 0.f);
    }
    inline static constexpr Vector3 Up() noexcept
    {
        return Vector3(0.f, 1.f, 0.f);
    }
    inline static constexpr Vector3 Down() noexcept
    {
        return Vector3(0.f, -1.f, 0.f);
    }
    inline static constexpr Vector3 Front() noexcept
    {
        return Vector3(0.f, 0.f, 1.f);
    }
    inline static constexpr Vector3 Back() noexcept
    {
        return Vector3(0.f, 0.f, -1.f);
    }

    inline constexpr Vector3 operator+() const noexcept
    {
        return *this;
    }
    inline constexpr Vector3 operator-() const noexcept
    {
        return Vector3(-x, -y, -z);
    }

    inline constexpr Vector3 operator+(const Vector3& rhs) const noexcept
    {
        return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    inline constexpr Vector3 operator-(const Vector3& rhs) const noexcept
    {
        return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    inline constexpr Vector3 operator*(const float s) const noexcept
    {
        return Vector3(x * s, y * s, z * s);
    }
    inline Vector3 operator/(const float s) const noexcept
    {
        if (Math::NearlyEqual(s, 0.f))
        {
            assert(false && "division by zero in Vector3::operator/");
            return Vector3::Zero();
        }
        return Vector3(x / s, y / s, z / s);
    }

    inline constexpr Vector3& operator+=(const Vector3& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    inline constexpr Vector3& operator-=(const Vector3& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    inline constexpr Vector3& operator*=(const float s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    inline Vector3& operator/=(const float s) noexcept
    {
        if (Math::NearlyEqual(s, 0.f))
        {
            assert(false && "division by zero in Vector3::operator/=");
            *this = Vector3::Zero();
            return *this;
        }
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    inline bool operator==(const Vector3& rhs) const noexcept
    {
        return Math::NearlyEqual(x, rhs.x) && Math::NearlyEqual(y, rhs.y) && Math::NearlyEqual(z, rhs.z);
    }
    inline bool operator!=(const Vector3& rhs) const noexcept
    {
        return !(*this == rhs);
    }
};
inline constexpr Vector3 operator*(float s, const Vector3& v) noexcept
{
    return Vector3(v.x * s, v.y * s, v.z * s);
}

struct Vector4
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;

    constexpr Vector4() noexcept : x(0.f), y(0.f), z(0.f), w(0.f) {}
    constexpr Vector4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}
    constexpr Vector4(const Vector3& v, float w) noexcept : x(v.x), y(v.y), z(v.z), w(w) {}

    inline bool IsZero() const noexcept
    {
        return Math::NearlyEqual(x, 0.f) && Math::NearlyEqual(y, 0.f) && Math::NearlyEqual(z, 0.f) && Math::NearlyEqual(w, 0.f);
    }

    inline static constexpr Vector4 Zero() noexcept
    {
        return Vector4(0.f, 0.f, 0.f, 0.f);
    }
    inline static constexpr Vector4 One() noexcept
    {
        return Vector4(1.f, 1.f, 1.f, 1.f);
    }

    inline constexpr Vector4 operator+() const noexcept
    {
        return *this;
    }
    inline constexpr Vector4 operator-() const noexcept
    {
        return Vector4(-x, -y, -z, -w);
    }

    inline constexpr Vector4 operator+(const Vector4& rhs) const noexcept
    {
        return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }
    inline constexpr Vector4 operator-(const Vector4& rhs) const noexcept
    {
        return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }
    inline constexpr Vector4 operator*(const float s) const noexcept
    {
        return Vector4(x * s, y * s, z * s, w * s);
    }
    inline Vector4 operator/(const float s) const noexcept
    {
        if (Math::NearlyEqual(s, 0.f))
        {
            assert(false && "division by zero in Vector4::operator/");
            return Vector4::Zero();
        }
        return Vector4(x / s, y / s, z / s, w / s);
    }

    inline constexpr Vector4& operator+=(const Vector4& rhs) noexcept
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    inline constexpr Vector4& operator-=(const Vector4& rhs) noexcept
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }
    inline constexpr Vector4& operator*=(const float s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }
    inline Vector4& operator/=(const float s) noexcept
    {
        if (Math::NearlyEqual(s, 0.f))
        {
            assert(false && "division by zero in Vector4::operator/=");
            *this = Vector4::Zero();
            return *this;
        }
        x /= s;
        y /= s;
        z /= s;
        w /= s;
        return *this;
    }

    inline bool operator==(const Vector4& rhs) const noexcept
    {
        return Math::NearlyEqual(x, rhs.x) && Math::NearlyEqual(y, rhs.y) && Math::NearlyEqual(z, rhs.z) && Math::NearlyEqual(w, rhs.w);
    }
    inline bool operator!=(const Vector4& rhs) const noexcept
    {
        return !(*this == rhs);
    }

};
inline constexpr Vector4 operator*(float s, const Vector4& v) noexcept
{
    return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}

struct Matrix4x4
{
    union 
    {
        struct
        {
            float m11, m12, m13, m14
                , m21, m22, m23, m24
                , m31, m32, m33, m34
                , m41, m42, m43, m44;
        };
        float m[4][4];
    };
};

struct Quaternion
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
    float w = 0.f;
};

}
