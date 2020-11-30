#pragma once
#include "hound/core/property.h"
#include "hound/core/math/math_functions.h"

template<typename T>
struct vec2
{
private:
	typedef unsigned int index_type;
	typedef T value_type;
	typedef T scalar_type;
	typedef float float_type;
	typedef vec2<value_type> this_type;

	value_type m_x_;
	value_type m_y_;

public:
	static this_type one();
	static this_type zero();
	static this_type right();
	static this_type up();
	

	vec2(value_type x, value_type y);
	vec2(value_type v);
	vec2() = default;

	HND_PROPERTY(x, value_type, m_x_)
	HND_PROPERTY(y, value_type, m_y_)
	
	value_type& operator[](index_type index);
	const value_type& operator[](index_type index) const;

	float_type magnitude();
	float_type sqr_magnitude();
	
	this_type normalized();
	
	this_type operator+(const this_type& v) const;
	this_type& operator+=(const this_type& v);
	
	this_type operator-(const this_type& v) const;
	this_type& operator-=(const this_type& v);

	this_type operator*(const this_type& v) const;
	this_type& operator*=(const this_type& v);

	this_type operator/(const this_type& v) const;
	this_type& operator/=(const this_type& v);

	this_type operator+(const scalar_type& scalar) const;
	this_type& operator+=(const scalar_type& scalar);

	this_type operator-(const scalar_type& scalar) const;
	this_type& operator-=(const scalar_type& scalar);

	this_type operator*(const scalar_type& scalar) const;
	this_type& operator*=(const scalar_type& scalar);

	this_type operator/(const scalar_type& scalar) const;
	this_type& operator/=(const scalar_type& scalar);
};

template <typename T>
typename vec2<T>::this_type vec2<T>::one()
{
	return this_type(static_cast<value_type>(1));
}

template <typename T>
typename vec2<T>::this_type vec2<T>::zero()
{
	return this_type(static_cast<value_type>(0));
}

template <typename T>
typename vec2<T>::this_type vec2<T>::right()
{
	return this_type(static_cast<value_type>(1), static_cast<value_type>(0));
}

template <typename T>
typename vec2<T>::this_type vec2<T>::up()
{
	return this_type(static_cast<value_type>(0), static_cast<value_type>(1));
}

template <typename T>
vec2<T>::vec2(value_type x, value_type y)
	:
	m_x_(x),
	m_y_(y)
{
}

template <typename T>
vec2<T>::vec2(value_type v)
	:
	m_x_(v),
	m_y_(v)
{
}

template <typename T>
typename vec2<T>::value_type& vec2<T>::operator[](index_type index)
{
	return reinterpret_cast<value_type*>(this)[index];
}

template <typename T>
const typename vec2<T>::value_type& vec2<T>::operator[](index_type index) const
{
	return reinterpret_cast<const value_type*>(this)[index];
}

template <typename T>
typename vec2<T>::float_type vec2<T>::magnitude()
{
	return math::sqrt(sqr_magnitude());
}

template <typename T>
typename vec2<T>::float_type vec2<T>::sqr_magnitude()
{
	return math::square(m_x_) + math::square(m_y_);
}

template <typename T>
typename vec2<T>::this_type vec2<T>::normalized()
{
	float_type mag = magnitude();
	value_type n_x = m_x_ / mag;
	value_type n_y = m_y_ / mag;
	return this_type(n_x, n_y);
}

template <typename T>
typename vec2<T>::this_type vec2<T>::operator+(const this_type& v) const
{
	return this_type(m_x_ + v.get_x(), m_y_ + v.get_y());
}

template <typename T>
typename vec2<T>::this_type& vec2<T>::operator+=(const this_type& v)
{
	m_x_ += v.get_x();
	m_y_ += v.get_y();
	return *this;
}

template <typename T>
typename vec2<T>::this_type vec2<T>::operator-(const this_type& v) const
{
	return this_type(m_x_ - v.get_x(), m_y_ - v.get_y());
}

template <typename T>
typename vec2<T>::this_type& vec2<T>::operator-=(const this_type& v)
{
	m_x_ -= v.get_x();
	m_y_ -= v.get_y();
	return *this;
}

template <typename T>
typename vec2<T>::this_type vec2<T>::operator*(const this_type& v) const
{
	return this_type(m_x_ * v.get_x(), m_y_ * v.get_y());
}

template <typename T>
typename vec2<T>::this_type& vec2<T>::operator*=(const this_type& v)
{
	m_x_ *= v.get_x();
	m_y_ *= v.get_y();
	return *this;
}

template <typename T>
typename vec2<T>::this_type vec2<T>::operator/(const this_type& v) const
{
	return this_type(m_x_ / v.get_x(), m_y_ / v.get_y());
}

template <typename T>
typename vec2<T>::this_type& vec2<T>::operator/=(const this_type& v)
{
	m_x_ /= v.get_x();
	m_y_ /= v.get_y();
	return *this;
}

template <typename T>
typename vec2<T>::this_type vec2<T>::operator+(const scalar_type& scalar) const
{
	return this_type(m_x_ + scalar, m_y_ + scalar);
}

template <typename T>
typename vec2<T>::this_type& vec2<T>::operator+=(const scalar_type& scalar)
{
	m_x_ += scalar;
	m_y_ += scalar;
	return *this;
}

template <typename T>
typename vec2<T>::this_type vec2<T>::operator-(const scalar_type& scalar) const
{
	return this_type(m_x_ - scalar, m_y_ - scalar);
}

template <typename T>
typename vec2<T>::this_type& vec2<T>::operator-=(const scalar_type& scalar)
{
	m_x_ -= scalar;
	m_y_ -= scalar;
	return *this;
}

template <typename T>
typename vec2<T>::this_type vec2<T>::operator*(const scalar_type& scalar) const
{
	return this_type(m_x_ * scalar, m_y_ * scalar);
}

template <typename T>
typename vec2<T>::this_type& vec2<T>::operator*=(const scalar_type& scalar)
{
	m_x_ *= scalar;
	m_y_ *= scalar;
	return *this;
}

template <typename T>
typename vec2<T>::this_type vec2<T>::operator/(const scalar_type& scalar) const
{
	return this_type(m_x_ / scalar, m_y_ / scalar);
}

template <typename T>
typename vec2<T>::this_type& vec2<T>::operator/=(const scalar_type& scalar)
{
	m_x_ /= scalar;
	m_y_ /= scalar;
	return *this;
}
