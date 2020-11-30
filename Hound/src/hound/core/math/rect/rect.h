#pragma once
#include "hound/core/property.h"

template<typename T>
struct vec2;

template<typename T>
struct rect
{
private:
	typedef T value_type;
	typedef T scalar_type;
	typedef float float_type;
	typedef rect<value_type> this_type;
	typedef vec2<value_type> vec2_type;
	typedef vec2<value_type> origin_type;
	typedef vec2<value_type> size_type;

	origin_type m_origin_;
	size_type m_size_;
	
public:
	static this_type zero();

	rect(value_type x, value_type y, value_type width, value_type height);
	rect(const vec2_type& position, const vec2_type& size);
	rect(value_type x, value_type y, const vec2_type& size);
	rect(const vec2_type& position, value_type width, value_type height);
	rect() = default;

	HND_PROPERTY(origin, origin_type, m_origin_)
	HND_PROPERTY(size, size_type, m_size_)
	
	HND_PROPERTY(center, size_type, m_origin_ + (m_size_ * static_cast<scalar_type>(0.5)))
	
	HND_PROPERTY_READ_ONLY(width, value_type, m_size_.get_x())
	HND_PROPERTY_READ_ONLY(height, value_type, m_size_.get_y())
	
	HND_PROPERTY_READ_ONLY(x, value_type, m_origin_.get_x())
	HND_PROPERTY_READ_ONLY(y, value_type, m_origin_.get_y())
	
	HND_PROPERTY_READ_ONLY(min, value_type, m_origin_)
	HND_PROPERTY_READ_ONLY(min_x, value_type, m_origin_.get_x())
	HND_PROPERTY_READ_ONLY(min_y, value_type, m_origin_.get_y())

	HND_PROPERTY_READ_ONLY(max, value_type, m_origin_ + m_size_)
	HND_PROPERTY_READ_ONLY(max_x, value_type, m_origin_.get_x() + m_size_.get_x())
	HND_PROPERTY_READ_ONLY(max_y, value_type, m_origin_.get_y() + m_size_.get_y())
};

template <typename T>
typename rect<T>::this_type rect<T>::zero()
{
	return rect(0, 0, 0, 0);
}

template <typename T>
rect<T>::rect(value_type x, value_type y, value_type width, value_type height) :
	m_origin_(x, y),
	m_size_(width, height)
{
}

template <typename T>
rect<T>::rect(const vec2_type& position, const vec2_type& size) :
	m_origin_(position),
	m_size_(size)
{
}

template <typename T>
rect<T>::rect(value_type x, value_type y, const vec2_type& size) :
	m_origin_(x, y),
	m_size_(size)
{
}

template <typename T>
rect<T>::rect(const vec2_type& position, value_type width, value_type height) :
	m_origin_(position),
	m_size_(width, height)
{
}
