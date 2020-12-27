#pragma once
#include "hound/core/property.h"
#include "hound/core/math/math_functions.h"

template<typename T>
struct vec2;

template<typename T>
struct vec3
{
private:
	typedef uint32_t index_type;
	typedef T value_type;
	typedef T scalar_type;
	typedef float float_type;
	typedef vec3<value_type> this_type;
	typedef vec2<value_type> vec2_type;

	value_type m_x_;
	value_type m_y_;
	value_type m_z_;
	
public:
	static this_type one();
	static this_type zero();
	static this_type right();
	static this_type up();
	static this_type forward();
	static float_type dot(const this_type& a, const this_type& b);
	static this_type cross(const this_type& a, const this_type& b);

	vec3(value_type x, value_type y, value_type z);
	vec3(const vec2_type& v, value_type z);
	vec3(value_type x, const vec2_type& v);
	vec3(value_type v);
	vec3() = default;

	HND_PROPERTY(x, value_type, m_x_)
	HND_PROPERTY(y, value_type, m_y_)
	HND_PROPERTY(z, value_type, m_z_)

	value_type& operator[](index_type index);
	const value_type& operator[](index_type index) const;

	[[nodiscard]] float_type magnitude() const;
	float_type sqr_magnitude() const;

	this_type normalized() const;

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
typename vec3<T>::this_type vec3<T>::one()
{
	return this_type(static_cast<value_type>(1));
}

template <typename T>
typename vec3<T>::this_type vec3<T>::zero()
{
	return this_type(static_cast<value_type>(0));
}

template <typename T>
typename vec3<T>::this_type vec3<T>::right()
{
	return this_type(static_cast<value_type>(1), static_cast<value_type>(0), static_cast<value_type>(0));
}

template <typename T>
typename vec3<T>::this_type vec3<T>::up()
{
	return this_type(static_cast<value_type>(0), static_cast<value_type>(1), static_cast<value_type>(0));
}

template <typename T>
typename vec3<T>::this_type vec3<T>::forward()
{
	return this_type(static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(1));
}

template <typename T>
typename vec3<T>::float_type vec3<T>::dot(const this_type& a, const this_type& b)
{
	value_type a_b = a.get_x() * b.get_x() + a.get_y() * b.get_y() + a.get_z() * b.get_z();
	
	const float_type a_mag = a.magnitude();
	const float_type b_mag = b.magnitude();

	return static_cast<float_type>(math::acos(a_b / (a_mag * b_mag)));
}

template <typename T>
typename vec3<T>::this_type vec3<T>::cross(const this_type& a, const this_type& b)
{
	value_type r_x = a.get_y() * b.get_z() - a.get_z() * b.get_y();
	value_type r_y = a.get_z() * b.get_x() - a.get_x() * b.get_z();
	value_type r_z = a.get_x() * b.get_y() - a.get_y() * b.get_x();

	return this_type(r_x, r_y, r_z);
}

template <typename T>
vec3<T>::vec3(value_type x, value_type y, value_type z) :
	m_x_(x),
	m_y_(y),
	m_z_(z)
{
}

template <typename T>
vec3<T>::vec3(const vec2_type& v, value_type z) :
	m_x_(v.get_x()),
	m_y_(v.get_y()),
	m_z_(z)
{
}

template <typename T>
vec3<T>::vec3(value_type x, const vec2_type& v) :
	m_x_(x),
	m_y_(v.get_x()),
	m_z_(v.get_y())
{
}

template <typename T>
vec3<T>::vec3(value_type v) :
	m_x_(v),
	m_y_(v),
	m_z_(v)
{
}

template <typename T>
typename vec3<T>::value_type& vec3<T>::operator[](index_type index)
{
	return reinterpret_cast<value_type*>(this)[index];
}

template <typename T>
const typename vec3<T>::value_type& vec3<T>::operator[](index_type index) const
{
	return reinterpret_cast<const value_type*>(this)[index];
}

template <typename T>
typename vec3<T>::float_type vec3<T>::magnitude() const
{
	return math::sqrt(sqr_magnitude());
}

template <typename T>
typename vec3<T>::float_type vec3<T>::sqr_magnitude() const
{
	return math::square(m_x_) + math::square(m_y_) + math::square(m_z_);
}

template <typename T>
typename vec3<T>::this_type vec3<T>::normalized() const
{
	float_type mag = magnitude();
	value_type n_x = m_x_ / mag;
	value_type n_y = m_y_ / mag;
	value_type n_z = m_z_ / mag;
	return this_type(n_x, n_y, n_z);
}

template <typename T>
typename vec3<T>::this_type vec3<T>::operator+(const this_type& v) const
{
	return this_type(m_x_ + v.get_x(), m_y_ + v.get_y(), m_z_ + v.get_z());
}

template <typename T>
typename vec3<T>::this_type& vec3<T>::operator+=(const this_type& v)
{
	m_x_ += v.get_x();
	m_y_ += v.get_y();
	m_z_ += v.get_z();
	return *this;
}

template <typename T>
typename vec3<T>::this_type vec3<T>::operator-(const this_type& v) const
{
	return this_type(m_x_ - v.get_x(), m_y_ - v.get_y(), m_z_ - v.get_z());
}

template <typename T>
typename vec3<T>::this_type& vec3<T>::operator-=(const this_type& v)
{
	m_x_ -= v.get_x();
	m_y_ -= v.get_y();
	m_z_ -= v.get_z();
	return *this;
}

template <typename T>
typename vec3<T>::this_type vec3<T>::operator*(const this_type& v) const
{
	return this_type(m_x_ * v.get_x(), m_y_ * v.get_y(), m_z_ * v.get_z());
}

template <typename T>
typename vec3<T>::this_type& vec3<T>::operator*=(const this_type& v)
{
	m_x_ *= v.get_x();
	m_y_ *= v.get_y();
	m_z_ *= v.get_z();
	return *this;
}

template <typename T>
typename vec3<T>::this_type vec3<T>::operator/(const this_type& v) const
{
	return this_type(m_x_ / v.get_x(), m_y_ / v.get_y(), m_z_ / v.get_z());
}

template <typename T>
typename vec3<T>::this_type& vec3<T>::operator/=(const this_type& v)
{
	m_x_ /= v.get_x();
	m_y_ /= v.get_y();
	m_z_ /= v.get_z();
	return *this;
}

template <typename T>
typename vec3<T>::this_type vec3<T>::operator+(const scalar_type& scalar) const
{
	return this_type(m_x_ + scalar, m_y_ + scalar, m_z_ + scalar);
}

template <typename T>
typename vec3<T>::this_type& vec3<T>::operator+=(const scalar_type& scalar)
{
	m_x_ += scalar;
	m_y_ += scalar;
	m_z_ += scalar;
	return *this;
}

template <typename T>
typename vec3<T>::this_type vec3<T>::operator-(const scalar_type& scalar) const
{
	return this_type(m_x_ - scalar, m_y_ - scalar, m_z_ - scalar);
}

template <typename T>
typename vec3<T>::this_type& vec3<T>::operator-=(const scalar_type& scalar)
{
	m_x_ -= scalar;
	m_y_ -= scalar;
	m_z_ -= scalar;
	return *this;
}

template <typename T>
typename vec3<T>::this_type vec3<T>::operator*(const scalar_type& scalar) const
{
	return this_type(m_x_ * scalar, m_y_ * scalar, m_z_ * scalar);
}

template <typename T>
typename vec3<T>::this_type& vec3<T>::operator*=(const scalar_type& scalar)
{
	m_x_ *= scalar;
	m_y_ *= scalar;
	m_z_ *= scalar;
	return *this;
}

template <typename T>
typename vec3<T>::this_type vec3<T>::operator/(const scalar_type& scalar) const
{
	return this_type(m_x_ / scalar, m_y_ / scalar, m_z_ / scalar);
}

template <typename T>
typename vec3<T>::this_type& vec3<T>::operator/=(const scalar_type& scalar)
{
	m_x_ /= scalar;
	m_y_ /= scalar;
	m_z_ /= scalar;
	return *this;
}
