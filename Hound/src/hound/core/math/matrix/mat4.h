#pragma once
#include "hound/core/math/math_functions.h"

template<typename T>
struct vec3;

template<typename T>
struct vec4;

template<typename T>
struct quaternion;

template<typename T>
struct mat4
{
private:
	typedef uint32_t index_type;
	typedef T value_type;
	typedef T scalar_type;
	typedef float float_type;
	typedef mat4<value_type> this_type;
	typedef vec4<value_type> vec4_type;
	typedef vec3<value_type> vec3_type;
	typedef quaternion<value_type> quaternion_type;
	
	typedef vec4<value_type> col_type;

	col_type m_values_[4];

public:
	static this_type create_identity();
	static this_type zero();
	static this_type from_quaternion(const quaternion_type& q);
	static this_type from_translation(const vec3_type& trans);
	static this_type from_scale(const vec3_type& scale);
	static this_type from_axis_angle(const vec3_type& axis, float_type angle);

	static this_type rotate(const this_type& mat, const vec3_type& axis, float_type angle);
	static this_type rotate(const this_type& mat, const quaternion_type& q);
	static this_type translate(const this_type& mat, const vec3_type& trans);
	static this_type scale(const this_type& mat, const vec3_type& scale);

	mat4(const col_type& c0, const col_type& c1, const col_type& c2, const col_type& c3);
	
	mat4(value_type _00, value_type _01, value_type _02, value_type _03,
		 value_type _10, value_type _11, value_type _12, value_type _13, 
		 value_type _20, value_type _21, value_type _22, value_type _23, 
		 value_type _30, value_type _31, value_type _32, value_type _33);

	mat4() = default;

	void set_identity();
	
	const col_type& operator[](index_type index) const;
	col_type& operator[](index_type index);

	this_type operator+(const this_type& m) const;
	this_type& operator+=(const this_type& m);

	this_type operator-(const this_type& m) const;
	this_type& operator-=(const this_type& m);

	this_type operator*(const this_type& m) const;
	this_type& operator*=(const this_type& m);

	vec4_type operator*(const vec4_type& v) const;

	vec3_type operator*(const vec3_type& v) const;
	
	this_type operator*(const scalar_type& s) const;
	this_type& operator*=(const scalar_type& s);

	this_type operator/(const scalar_type& s) const;
	this_type& operator/=(const scalar_type& s);

	const value_type* get_value_ptr() const;
};

template <typename T>
typename mat4<T>::this_type mat4<T>::create_identity()
{
	return this_type(
		{ static_cast<value_type>(1), static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0) },
		{ static_cast<value_type>(0), static_cast<value_type>(1), static_cast<value_type>(0), static_cast<value_type>(0) },
		{ static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(1), static_cast<value_type>(0) },
		{ static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(1) }
		);
}

template <typename T>
typename mat4<T>::this_type mat4<T>::zero()
{
	return this_type(
		{ static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0) },
		{ static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0) },
		{ static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0) },
		{ static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0), static_cast<value_type>(0) }
	);
}

//https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
template <typename T>
typename mat4<T>::this_type mat4<T>::from_quaternion(const quaternion_type& q)
{
	value_type x = q.get_x();
	value_type y = q.get_y();
	value_type z = q.get_z();
	value_type w = q.get_w();


	value_type _00 = 1 - 2 * (y * y + z * z);
	value_type _01 = 2 * (x * y - z * w);
	value_type _02 = 2 * (x * z + y * w);
	value_type _03 = 0;

	value_type _10 = 2 * (x * y + z * w);
	value_type _11 = 1 - 2 * (x * x + z * z);
	value_type _12 = 2 * (x * x - z * z);
	value_type _13 = 0;

	value_type _20 = 2 * (x * z - y * w);
	value_type _21 = 2 * (y * z + x * w);
	value_type _22 = 1 - 2 * (x * x + y * y);
	value_type _23 = 0;

	value_type _30 = 0;
	value_type _31 = 0;
	value_type _32 = 0;
	value_type _33 = 1;

	return this_type(
		{ _00, _01, _02, _03 },
		{ _10, _11, _12, _13 },
		{ _20, _21, _22, _23 },
		{ _30, _31, _32, _33 }
	);
}

template <typename T>
typename mat4<T>::this_type mat4<T>::from_translation(const vec3_type& trans)
{
	this_type mat = create_identity();
	mat[3][0] = trans.get_x();
	mat[3][1] = trans.get_y();
	mat[3][2] = trans.get_z();
	return mat;
}

template <typename T>
typename mat4<T>::this_type mat4<T>::from_scale(const vec3_type& scale)
{
	this_type mat = create_identity();
	mat[0][0] *= scale.get_x();
	mat[1][1] *= scale.get_y();
	mat[2][2] *= scale.get_z();
	return mat;
}

template <typename T>
typename mat4<T>::this_type mat4<T>::from_axis_angle(const vec3_type& axis, float_type angle)
{
	vec3_type norm_axis = axis.normalized();
	value_type c = math::cos(angle * math::DEG_RAD_CONST);
	value_type s = math::sin(angle * math::DEG_RAD_CONST);
	value_type t = 1 - c;

	value_type _00 = c + norm_axis.get_x() * norm_axis.get_x() * t;
	value_type _11 = c + norm_axis.get_y() * norm_axis.get_y() * t;
	value_type _22 = c + norm_axis.get_z() * norm_axis.get_z() * t;
	value_type _33 = 1;

	value_type tmp_1 = norm_axis.get_x() * norm_axis.get_y() * t;
	value_type tmp_2 = norm_axis.get_z() * s;

	value_type _10 = tmp_1 + tmp_2;
	value_type _01 = tmp_1 - tmp_2;

	tmp_1 = norm_axis.get_x() * norm_axis.get_z() * t;
	tmp_2 = norm_axis.get_y() * s;

	value_type _20 = tmp_1 - tmp_2;
	value_type _02 = tmp_1 + tmp_2;

	tmp_1 = norm_axis.get_y() * norm_axis.get_z() * t;
	tmp_2 = norm_axis.get_x() * s;

	value_type _21 = tmp_1 + tmp_2;
	value_type _12 = tmp_1 - tmp_2;

	value_type _03 = 0;
	value_type _13 = 0;
	value_type _23 = 0;
	value_type _30 = 0;
	value_type _31 = 0;
	value_type _32 = 0;

	return this_type(
		{ _00, _01, _02, _03 },
		{ _10, _11, _12, _13 },
		{ _20, _21, _22, _23 },
		{ _30, _31, _32, _33 }
	);
}

template <typename T>
typename mat4<T>::this_type mat4<T>::rotate(const this_type& mat, const vec3_type& axis, float_type angle)
{
	this_type rotated_mat = from_axis_angle(axis, angle);
	return mat * rotated_mat;
}

template <typename T>
typename mat4<T>::this_type mat4<T>::rotate(const this_type& mat, const quaternion_type& q)
{
	this_type rotated_mat = from_quaternion(q);
	return mat * rotated_mat;
}

template <typename T>
typename mat4<T>::this_type mat4<T>::translate(const this_type& mat, const vec3_type& trans)
{
	this_type translated_mat = from_translation(trans);
	return mat * translated_mat;
}

template <typename T>
typename mat4<T>::this_type mat4<T>::scale(const this_type& mat, const vec3_type& scale)
{
	this_type scaled_mat = from_scale(scale);
	return mat * scaled_mat;
}

template <typename T>
mat4<T>::mat4(const col_type& c0, const col_type& c1, const col_type& c2, const col_type& c3) :
	m_values_{c0, c1, c2, c3}
{
}

template <typename T>
mat4<T>::mat4(	value_type _00, value_type _01, value_type _02, value_type _03, 
				value_type _10, value_type _11, value_type _12, value_type _13, 
				value_type _20, value_type _21, value_type _22, value_type _23, 
				value_type _30,	value_type _31, value_type _32, value_type _33) :
	m_values_{
		{ _00, _01, _02, _03 },
		{ _10, _11, _12, _13 },
		{ _20, _21, _22, _23 },
		{ _30, _31, _32, _33 }
	}
{
}

template <typename T>
void mat4<T>::set_identity()
{
	m_values_[0][0] = static_cast<value_type>(1);
	m_values_[0][1] = static_cast<value_type>(0);
	m_values_[0][2] = static_cast<value_type>(0);
	m_values_[0][3] = static_cast<value_type>(0);

	m_values_[1][0] = static_cast<value_type>(0);
	m_values_[1][1] = static_cast<value_type>(1);
	m_values_[1][2] = static_cast<value_type>(0);
	m_values_[1][3] = static_cast<value_type>(0);

	m_values_[2][0] = static_cast<value_type>(0);
	m_values_[2][1] = static_cast<value_type>(0);
	m_values_[2][2] = static_cast<value_type>(1);
	m_values_[2][3] = static_cast<value_type>(0);

	m_values_[3][0] = static_cast<value_type>(0);
	m_values_[3][1] = static_cast<value_type>(0);
	m_values_[3][2] = static_cast<value_type>(0);
	m_values_[3][3] = static_cast<value_type>(1);
}

template <typename T>
const typename mat4<T>::col_type& mat4<T>::operator[](index_type index) const
{
	return m_values_[index];
}

template <typename T>
typename mat4<T>::col_type& mat4<T>::operator[](index_type index)
{
	return m_values_[index];
}

template <typename T>
typename mat4<T>::this_type mat4<T>::operator+(const this_type& m) const
{
	return this_type(m_values_[0] + m.m_values_[0], 
					 m_values_[1] + m.m_values_[1], 
					 m_values_[2] + m.m_values_[2],
					 m_values_[3] + m.m_values_[3]);
}

template <typename T>
typename mat4<T>::this_type& mat4<T>::operator+=(const this_type& m)
{
	m_values_[0] += m.m_values_[0];
	m_values_[1] += m.m_values_[1];
	m_values_[2] += m.m_values_[2];
	m_values_[3] += m.m_values_[3];
	return *this;
}

template <typename T>
typename mat4<T>::this_type mat4<T>::operator-(const this_type& m) const
{
	return this_type(m_values_[0] - m.m_values_[0],
					 m_values_[1] - m.m_values_[1],
					 m_values_[2] - m.m_values_[2],
					 m_values_[3] - m.m_values_[3]);
}

template <typename T>
typename mat4<T>::this_type& mat4<T>::operator-=(const this_type& m)
{
	m_values_[0] -= m.m_values_[0];
	m_values_[1] -= m.m_values_[1];
	m_values_[2] -= m.m_values_[2];
	m_values_[3] -= m.m_values_[3];
	return *this;
}

template <typename T>
typename mat4<T>::this_type mat4<T>::operator*(const this_type& m) const
{
	this_type result;
	result[0] = m_values_[0] * m[0][0] + m_values_[1] * m[0][1] + m_values_[2] * m[0][2] + m_values_[3] * m[0][3];
	result[1] = m_values_[0] * m[1][0] + m_values_[1] * m[1][1] + m_values_[2] * m[1][2] + m_values_[3] * m[1][3];
	result[2] = m_values_[0] * m[2][0] + m_values_[1] * m[2][1] + m_values_[2] * m[2][2] + m_values_[3] * m[2][3];
	result[3] = m_values_[0] * m[3][0] + m_values_[1] * m[3][1] + m_values_[2] * m[3][2] + m_values_[3] * m[3][3];
	return result;
}

template <typename T>
typename mat4<T>::this_type& mat4<T>::operator*=(const this_type& m)
{
	this_type this_copy = *this;
	m_values_[0] = this_copy[0] * m[0][0] + this_copy[1] * m[0][1] + this_copy[2] * m[0][2] + this_copy[3] * m[0][3];
	m_values_[1] = this_copy[0] * m[1][0] + this_copy[1] * m[1][1] + this_copy[2] * m[1][2] + this_copy[3] * m[1][3];
	m_values_[2] = this_copy[0] * m[2][0] + this_copy[1] * m[2][1] + this_copy[2] * m[2][2] + this_copy[3] * m[2][3];
	m_values_[3] = this_copy[0] * m[3][0] + this_copy[1] * m[3][1] + this_copy[2] * m[3][2] + this_copy[3] * m[3][3];
	return *this;
}

template <typename T>
typename mat4<T>::vec4_type mat4<T>::operator*(const vec4_type& v) const
{
	vec4_type mov0(v[0]);
	vec4_type mov1(v[1]);
	vec4_type mul0 = m_values_[0] * mov0;
	vec4_type mul1 = m_values_[1] * mov1;
	vec4_type add0 = mul0 + mul1;

	vec4_type mov2(v[2]);
	vec4_type mov3(v[3]);
	vec4_type mul2 = m_values_[2] * mov2;
	vec4_type mul3 = m_values_[3] * mov3;
	vec4_type add1 = mul2 + mul3;
	
	vec4_type add2 = add0 + add1;
	
	return add2;
}

template <typename T>
typename mat4<T>::vec3_type mat4<T>::operator*(const vec3_type& v) const
{
	return *this * vec4_type(v, 1);
}

template <typename T>
typename mat4<T>::this_type mat4<T>::operator*(const scalar_type& s) const
{
	return this_type(m_values_[0] * s,
					 m_values_[1] * s,
					 m_values_[2] * s,
					 m_values_[3] * s);
}

template <typename T>
typename mat4<T>::this_type& mat4<T>::operator*=(const scalar_type& s)
{
	m_values_[0] *= s;
	m_values_[1] *= s;
	m_values_[2] *= s;
	m_values_[3] *= s;
	return *this;
}

template <typename T>
typename mat4<T>::this_type mat4<T>::operator/(const scalar_type& s) const
{
	return this_type(m_values_[0] / s,
					 m_values_[1] / s,
					 m_values_[2] / s,
					 m_values_[3] / s);
}

template <typename T>
typename mat4<T>::this_type& mat4<T>::operator/=(const scalar_type& s)
{
	m_values_[0] /= s;
	m_values_[1] /= s;
	m_values_[2] /= s;
	m_values_[3] /= s;
	return *this;
}

template <typename T>
const typename mat4<T>::value_type* mat4<T>::get_value_ptr() const
{
	return reinterpret_cast<const value_type*>(this);
}
