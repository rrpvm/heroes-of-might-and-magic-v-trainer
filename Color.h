#pragma once

class Color
{
	int r, g, b, a;
public:
	Color(int r = 0, int  g = 0, int b = 0, int a = 255)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	const unsigned long to_dword() const
	{
		return (unsigned long)(((this->a & 255) << 24) | ((this->r & 255) << 16) | ((this->g & 255) << 8) | ((this->b & 255)));
	}
	static Color black() {
		return Color();
	}
	static Color white() {
		return Color(255, 255, 255);
	}
	static Color red() {
		return Color(255, 0, 0);
	}
	static Color green() {
		return Color(0, 255, 0);
	}
	void clamp() {
		if (this->r > 255)this->r = 255;
		else if (this->r < 0)this->r = 0;
		if (this->g > 255)this->g = 255;
		else if (this->g < 0)this->g = 0;
		if (this->b > 255)this->b = 255;
		else if (this->b < 0)this->b = 0;
		if (this->a > 255)this->a = 255;
		else if (this->a < 0)this->a = 0;
		/*can be replaced by ternar operators: r > 255.f ? r = 255.f : r < 0.0f ? r = 0.0f : r = r*/
	}
	static Color gray_m() {//mid
		return Color(125, 125, 125);
	}
	static Color gray_b() {//black
		return Color(65, 65, 65);
	}
	static Color gray_w() {//white
		return Color(185, 185, 185);
	}
	void set_color(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	void set_color(int r, int g, int b, int a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	void set_alpha(int a)
	{
		this->a = a;
	}
	Color& operator*=(int x)
	{
		this->r *= x;
		this->g *= x;
		this->b *= x;
		this->a *= x;
		this->clamp();
		return *this;
	}
	Color& operator*(Color x)
	{
		this->r *= x.r;
		this->g *= x.g;
		this->b *= x.b;
		this->a *= x.a;
		this->clamp();
		return *this;
	}
	Color& operator/(Color x)
	{
		this->r /= x.r;
		this->g /= x.g;
		this->b /= x.b;
		this->a /= x.a;
		this->clamp();
		return *this;
	}
	Color& operator-(Color x)
	{
		this->r -= x.r;
		this->g -= x.g;
		this->b -= x.b;
		this->a -= x.a;
		this->clamp();
		return *this;
	}
	Color& operator+(Color x)
	{
		this->r += x.r;
		this->g += x.g;
		this->b += x.b;
		this->a += x.a;
		this->clamp();
		return *this;
	}
	Color& operator=(Color a)
	{
		this->r = a.r;
		this->g = a.g;
		this->b = a.b;
		this->a = a.a;
		return *this;
	}
	int& operator[](int index);

private:
};
