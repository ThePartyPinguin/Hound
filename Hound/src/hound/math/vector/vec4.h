#pragma once
#include "hound/core/property.h"
#include "hound/math/math_functions.h"

template<typename T>
struct vec2;

template<typename T>
struct vec3;

template<typename T>
struct vec4
{
private:
	typedef uint32_t index_type;
	typedef T value_type;
	typedef T scalar_type;
	typedef float float_type;
	typedef vec4<value_type> this_type;
	typedef vec3<value_type> vec3_type;
	typedef vec2<value_type> vec2_type;

	value_type m_x_;
	value_type m_y_;
	value_type m_z_;
	value_type m_w_;

public:
	static this_type one();
	static this_type zero();

	vec4(value_type x, value_type y, value_type z, value_type w);

	vec4(vec2_type v, value_type z, value_type w);
	vec4(value_type x, vec2_type v, value_type w);
	vec4(value_type x, value_type y, vec2_type v);

	vec4(vec2_type v_0, vec2_type v_1);
	
	vec4(value_type x, vec3_type v);
	vec4(vec3_type v, value_type w);
	vec4(value_type v);
	
	vec4() = default;

	HND_PROPERTY(x, value_type, m_x_)
	HND_PROPERTY(y, value_type, m_y_)
	HND_PROPERTY(z, value_type, m_z_)
	HND_PROPERTY(w, value_type, m_w_)

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
typename vec4<T>::this_type vec4<T>::one()
{
	return this_type(static_cast<value_type>(1));
}

template <typename T>
typename vec4<T>::this_type vec4<T>::zero()
{
	return this_type(static_cast<value_type>(0));
}

template <typename T>
vec4<T>::vec4(value_type x, value_type y, value_type z, value_type w) :
	m_x_(x),
	m_y_(y),
	m_z_(z),
	m_w_(w)
{
}

template <typename T>
vec4<T>::vec4(vec2_type v, value_type z, value_type w) :
	m_x_(v.get_x()),
	m_y_(v.get_y()),
	m_z_(z),
	m_w_(w)
{
}

template <typename T>
vec4<T>::vec4(value_type x, vec2_type v, value_type w) :
	m_x_(x),
	m_y_(v.get_x()),
	m_z_(v.get_y()),
	m_w_(w)
{
}

template <typename T>
vec4<T>::vec4(value_type x, value_type y, vec2_type v) :
	m_x_(x),
	m_y_(y),
	m_z_(v.get_x()),
	m_w_(v.get_y())
{
}

template <typename T>
vec4<T>::vec4(vec2_type v_0, vec2_type v_1) :
	m_x_(v_0.get_x()),
	m_y_(v_0.get_y()),
	m_z_(v_1.get_x()),
	m_w_(v_1.get_y())
{
}

template <typename T>
vec4<T>::vec4(value_type x, vec3_type v) :
	m_x_(x),
	m_y_(v.get_x()),
	m_z_(v.get_y()),
	m_w_(v.get_z())
{
}

template <typename T>
vec4<T>::vec4(vec3_type v, value_type w) :
	m_x_(v.get_x()),
	m_y_(v.get_y()),
	m_z_(v.get_z()),
	m_w_(w)
{
}

template <typename T>
vec4<T>::vec4(value_type v) :
	m_x_(v),
	m_y_(v),
	m_z_(v),
	m_w_(v)
{
}

template <typename T>
typename vec4<T>::value_type& vec4<T>::operator[](index_type index)
{
	return reinterpret_cast<value_type*>(this)[index];
}

template <typename T>
const typename vec4<T>::value_type& vec4<T>::operator[](index_type index) const
{
	return reinterpret_cast<const value_type*>(this)[index];
}

template <typename T>
typename vec4<T>::float_type vec4<T>::magnitude()
{
	return math::sqrt(sqr_magnitude());
}

template <typename T>
typename vec4<T>::float_type vec4<T>::sqr_magnitude()
{
	return math::square(m_x_) + math::square(m_y_) + math::square(m_z_) + math::square(m_w_);
}

template <typename T>
typename vec4<T>::this_type vec4<T>::normalized()
{
	float_type mag = magnitude();
	value_type n_x = m_x_ / mag;
	value_type n_y = m_y_ / mag;
	value_type n_z = m_z_ / mag;
	value_type n_w = m_w_ / mag;
	return this_type(n_x, n_y, n_z);
}

template <typename T>
typename vec4<T>::this_type vec4<T>::operator+(const this_type& v) const
{
	return this_type(m_x_ + v.get_x(), m_y_ + v.get_y(), m_z_ + v.get_z(), m_w_ + v.get_w());
}

template <typename T>
typename vec4<T>::this_type& vec4<T>::operator+=(const this_type& v)
{
	m_x_ += v.get_x();
	m_y_ += v.get_y();
	m_z_ += v.get_z();
	m_w_ += v.get_w();
	return *this;
}

template <typename T>
typename vec4<T>::this_type vec4<T>::operator-(const this_type& v) const
{
	return this_type(m_x_ - v.get_x(), m_y_ - v.get_y(), m_z_ - v.get_z(), m_w_ - v.get_w());
}

template <typename T>
typename vec4<T>::this_type& vec4<T>::operator-=(const this_type& v)
{
	m_x_ -= v.get_x();
	m_y_ -= v.get_y();
	m_z_ -= v.get_z();
	m_w_ -= v.get_w();
	return *this;
}

template <typename T>
typename vec4<T>::this_type vec4<T>::operator*(const this_type& v) const
{
	return this_type(m_x_ * v.get_x(), m_y_ * v.get_y(), m_z_ * v.get_z(), m_w_ * v.get_w());
}

template <typename T>
typename vec4<T>::this_type& vec4<T>::operator*=(const this_type& v)
{
	m_x_ *= v.get_x();
	m_y_ *= v.get_y();
	m_z_ *= v.get_z();
	m_w_ *= v.get_w();
	return *this;
}

template <typename T>
typename vec4<T>::this_type vec4<T>::operator/(const this_type& v) const
{
	return this_type(m_x_ / v.get_x(), m_y_ / v.get_y(), m_z_ / v.get_z(), m_w_ / v.get_w());
}

template <typename T>
typename vec4<T>::this_type& vec4<T>::operator/=(const this_type& v)
{
	m_x_ /= v.get_x();
	m_y_ /= v.get_y();
	m_z_ /= v.get_z();
	m_w_ /= v.get_w();
	return *this;
}

template <typename T>
typename vec4<T>::this_type vec4<T>::operator+(const scalar_type& scalar) const
{
	return this_type(m_x_ + scalar, m_y_ + scalar, m_z_ + scalar, m_w_ + scalar);
}

template <typename T>
typename vec4<T>::this_type& vec4<T>::operator+=(const scalar_type& scalar)
{
	m_x_ += scalar;
	m_y_ += scalar;
	m_z_ += scalar;
	m_w_ += scalar;
	return *this;
}

template <typename T>
typename vec4<T>::this_type vec4<T>::operator-(const scalar_type& scalar) const
{
	return this_type(m_x_ - scalar, m_y_ - scalar, m_z_ - scalar, m_w_ - scalar);
}

template <typename T>
typename vec4<T>::this_type& vec4<T>::operator-=(const scalar_type& scalar)
{
	m_x_ -= scalar;
	m_y_ -= scalar;
	m_z_ -= scalar;
	m_w_ -= scalar;
	return *this;
}

template <typename T>
typename vec4<T>::this_type vec4<T>::operator*(const scalar_type& scalar) const
{
	return this_type(m_x_ * scalar, m_y_ * scalar, m_z_ * scalar, m_w_ * scalar);
}

template <typename T>
typename vec4<T>::this_type& vec4<T>::operator*=(const scalar_type& scalar)
{
	m_x_ *= scalar;
	m_y_ *= scalar;
	m_z_ *= scalar;
	m_w_ *= scalar;
	return *this;
}

template <typename T>
typename vec4<T>::this_type vec4<T>::operator/(const scalar_type& scalar) const
{
	return this_type(m_x_ / scalar, m_y_ / scalar, m_z_ / scalar, m_w_ / scalar);
}

template <typename T>
typename vec4<T>::this_type& vec4<T>::operator/=(const scalar_type& scalar)
{
	m_x_ /= scalar;
	m_y_ /= scalar;
	m_z_ /= scalar;
	m_w_ /= scalar;
	return *this;
}

