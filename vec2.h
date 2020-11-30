#pragma once
class vec2
{
public:
	float x;
	float y;
	vec2(): x(0) , y(0) {};
	vec2(float x, float y): x(x) , y(y) {};
	vec2& operator+(int a)
	{
		this->x += a;
		this->y += a;
		return *this;
	}
	vec2& operator+(const vec2& v_other)
	{
		this->x += v_other.x;
		this->y += v_other.y;
		return *this;
	}

private:
};

