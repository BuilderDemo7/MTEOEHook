#pragma once

class Vector4 {
public:
	float X, Y, Z, W;

	void operator*(Vector4 other);
	void operator*(float multiplier);

	Vector4();
	Vector4(float x, float y, float z);
	Vector4(float x, float y, float z, float w);
};