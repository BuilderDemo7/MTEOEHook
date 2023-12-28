#include "Vector4.h"

Vector4::Vector4()
{
	X = Y = Z = W = 0.0f;
}

Vector4::Vector4(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
	W = 1.0f;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

void Vector4::operator*(Vector4 other)
{
	X *= other.X;
	Y *= other.Y;
	Z *= other.Z;
	W *= other.W;
}

void Vector4::operator*(float multiplier)
{
	X *= multiplier;
	Y *= multiplier;
	Z *= multiplier;
	W *= multiplier;
}