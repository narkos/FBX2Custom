#pragma once

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Vector2
{
public:
	float x;
	float y;

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

class Color
{
public:
	short r;
	short g;
	short b;
	short a;

	Color(short r, short g, short b, short a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};