#pragma once
#include <math.h>

class Vector3D
{
public:
	float x, y, z;
	Vector3D(void) {}
	Vector3D(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
	float Heading(void) const { return atan2(-x, y); }
	float Magnitude(void) const { return sqrt(x * x + y * y + z * z); }
	float MagnitudeSqr(void) const { return x * x + y * y + z * z; }
	float Magnitude2D(void) const { return sqrt(x * x + y * y); }
	float MagnitudeSqr2D(void) const { return x * x + y * y; }

	const Vector3D& operator+=(Vector3D const& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}

	const Vector3D& operator+=(float const& right) {
		x += right;
		y += right;
		z += right;
		return *this;
	}

	const Vector3D& operator-=(Vector3D const& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	const Vector3D& operator*=(float right) {
		x *= right;
		y *= right;
		z *= right;
		return *this;
	}

	const Vector3D& operator/=(float right) {
		x /= right;
		y /= right;
		z /= right;
		return *this;
	}

	Vector3D operator-() const {
		return Vector3D(-x, -y, -z);
	}
};

inline Vector3D operator+(const Vector3D& left, const Vector3D& right)
{
	return Vector3D(left.x + right.x, left.y + right.y, left.z + right.z);
}

inline Vector3D operator-(const Vector3D& left, const Vector3D& right)
{
	return Vector3D(left.x - right.x, left.y - right.y, left.z - right.z);
}

inline Vector3D operator*(const Vector3D& left, float right)
{
	return Vector3D(left.x * right, left.y * right, left.z * right);
}

inline Vector3D operator*(float left, const Vector3D& right)
{
	return Vector3D(left * right.x, left * right.y, left * right.z);
}

inline Vector3D operator/(const Vector3D& left, float right)
{
	return Vector3D(left.x / right, left.y / right, left.z / right);
}


inline float
Distance(const Vector3D& v1, const Vector3D& v2)
{
	return (v2 - v1).Magnitude();
}
