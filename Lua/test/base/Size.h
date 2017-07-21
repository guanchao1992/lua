#pragma once

class Size
{
public:

	Size()
		:m_width(.0f)
		, m_height(.0f)
	{
	}

	Size(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	Size(const Size&size)
	{
		*this = size;
	}

	~Size()
	{
	}

	inline void setSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	inline void setSize(const Size&size)
	{
		*this = size;
	}

	inline void setWidth(float width)
	{
		m_width = width;
	}

	inline void setHeight(float height)
	{
		m_height = height;
	}

	inline float getWidth() const
	{
		return m_width;
	}

	inline float getHeight() const
	{
		return m_height;
	}

	inline Size operator-() const
	{
		return Size(-this->m_width, -this->m_height);
	}

	inline Size& operator=(const Size& size)
	{
		this->m_width = size.m_width;
		this->m_height = size.m_height;
		return *this;
	}

	inline Size operator+(const Size& size) const
	{
		return Size(this->m_width + size.m_width, this->m_height + size.m_height);
	}

	inline Size operator-(const Size& size) const
	{
		return Size(this->m_width - size.m_width, this->m_height - size.m_height);
	}

	inline bool operator==(const Size& size) const
	{
		return (this->m_width == size.m_width && this->m_height == size.m_height);
	}
private:
	float m_width;
	float m_height;
};

