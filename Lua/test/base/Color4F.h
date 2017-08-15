#pragma once

class Color4F
{
public:

	Color4F()
		:r(1.f), g(1.f), b(1.f), w(1.f)
	{
	}

	Color4F(float r, float g, float b, float w)
		:r(r), g(g), b(b), w(w)
	{
	}

	Color4F(unsigned int rgb)
	{
		r = (rgb >> 24) / 256.0f;
		g = (rgb >> 16) % 256 / 256.0f;
		b = (rgb >> 8) % 256 / 256.0f;
		w = rgb % 256 / 256.0f;
	}

	Color4F(const Color4F&Color4F)
	{
		*this = Color4F;
	}

	~Color4F()
	{
	}

	inline void setColor4F(unsigned int rgb)
	{
		r = (rgb >> 24) / 256.0f;
		g = (rgb >> 16) % 256 / 256.0f;
		b = (rgb >> 8) % 256 / 256.0f;
		w = rgb % 256 / 256.0f;
	}

	inline Color4F& operator=(const Color4F& cl)
	{
		this->r = cl.r;
		this->g = cl.g;
		this->b = cl.b;
		this->w = cl.w;
		return *this;
	}

	inline bool operator==(const Color4F& cl) const
	{
		return (this->r == cl.r &&this->g == cl.g && this->b == cl.b && this->w == cl.w);
	}
public:
	float	r;
	float	g;
	float	b;
	float	w;
};

