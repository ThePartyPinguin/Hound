#pragma once
#include "hound/core/property.h"
#include "hound/math/math_functions.h"

template<typename T>
struct vec3;

template<typename T>
struct vec2;

template<typename T>
struct quaternion
{
private:
	typedef uint32_t index_type;
	typedef T value_type;
	typedef T scalar_type;
	typedef float float_type;
	typedef quaternion<value_type> this_type;
	typedef vec3<value_type> vec3_type;
	typedef vec2<value_type> vec2_type;

	value_type m_x_;
	value_type m_y_;
	value_type m_z_;
	value_type m_w_;

public:
	static this_type from_euler(const vec3_type& euler);
	static this_type from_euler(value_type x, value_type y, value_type z);
	static vec3_type to_euler(const this_type& q);
	static this_type from_axis_angle(const vec3_type& axis, float_type angle);
	static this_type create_identity();

	HND_PROPERTY(x, value_type, m_x_)
	HND_PROPERTY(y, value_type, m_y_)
	HND_PROPERTY(z, value_type, m_z_)
	HND_PROPERTY(w, value_type, m_w_)

	quaternion(value_type x, value_type y, value_type z, value_type w);
	quaternion(vec2_type v, value_type z, value_type w);
	quaternion(value_type x, vec2_type v, value_type w);
	quaternion(value_type x, value_type y, vec2_type v);
	quaternion(vec2_type v_0, vec2_type v_1);
	quaternion(value_type x, vec3_type v);
	quaternion(vec3_type v, value_type w);
	quaternion(value_type v);
	quaternion() = default;

	this_type operator*(const this_type& q) const;
	this_type& operator*=(const this_type& q);

	vec3_type operator*(const vec3_type& v) const;

	this_type inverse() const;

	[[nodiscard]] float_type magnitude() const;
	
	this_type normalized() const;
};

template <typename T>
typename quaternion<T>::this_type quaternion<T>::from_euler(const vec3_type& euler)
{
	return from_euler(euler.get_x(), euler.get_y(), euler.get_z());
}

template <typename T>
typename quaternion<T>::this_type quaternion<T>::from_euler(value_type x, value_type y, value_type z)
{
	value_type half_x = x * 0.5f * math::DEG_RAD_CONST;
	value_type half_y = y * 0.5f * math::DEG_RAD_CONST;
	value_type half_z = z * 0.5f * math::DEG_RAD_CONST;

	value_type cos_x = static_cast<value_type>(math::cos(half_x));
	value_type sin_x = static_cast<value_type>(math::sin(half_x));
	value_type cos_y = static_cast<value_type>(math::cos(half_y));
	value_type sin_y = static_cast<value_type>(math::sin(half_y));
	value_type cos_z = static_cast<value_type>(math::cos(half_z));
	value_type sin_z = static_cast<value_type>(math::sin(half_z));

	value_type q_x = sin_z * cos_x * cos_y - cos_z * sin_x * sin_y;
	value_type q_y = cos_z * sin_x * cos_y + sin_z * cos_x * sin_y;
	value_type q_z = cos_z * cos_x * sin_y - sin_z * sin_x * cos_y;
	value_type q_w = cos_z * cos_x * cos_y + sin_z * sin_x * sin_y;

	return this_type(q_x, q_y, q_z, q_w).normalized();
}

template <typename T>
typename quaternion<T>::vec3_type quaternion<T>::to_euler(const this_type& q)
{
	value_type q_x = q.get_x();
	value_type q_y = q.get_y();
	value_type q_z = q.get_z();
	value_type q_w = q.get_w();

	value_type eul_x;

	value_type sin_x = 2 * (q_w * q_y - q_z * q_x);

	if (math::abs(sin_x) >= 1)
	{
		eul_x = static_cast<value_type>(math::copy_sign(math::HALF_PI, sin_x));
	}
	else
	{
		eul_x = static_cast<value_type>(math::asin(sin_x));
	}

	value_type sin_z_cos_x = 2 * (q_w * q_x + q_y * q_z);
	value_type cos_z_cos_x = 1 - 2 * (q_x * q_x + q_y * q_y);

	value_type eul_z = static_cast<value_type>(math::atan2(sin_z_cos_x, cos_z_cos_x));

	value_type sin_y_cos_x = 2 * (q_w * q_z + q_x * q_y);
	value_type cos_y_cos_x = 1 - 2 * (q_y * q_y + q_z * q_z);

	value_type eul_y = static_cast<value_type>(math::atan2(sin_y_cos_x, cos_y_cos_x));

	return Vec3<value_type>(eul_x * math::RAD_DEG_CONST, eul_y * math::RAD_DEG_CONST, eul_z * math::RAD_DEG_CONST);
}

template <typename T>
typename quaternion<T>::this_type quaternion<T>::from_axis_angle(const vec3_type& axis, float_type angle)
{
	const float_type half_angle = angle * 0.5f * math::DEG_RAD_CONST;
	const float_type s = math::sin(half_angle);
	
	value_type q_x = axis.get_x() * s;
	value_type q_y = axis.get_y() * s;
	value_type q_z = axis.get_x() * s;
	value_type q_w = math::cos(half_angle);
	
	return this_type(q_x, q_y, q_z, q_w).normalized();
}

template <typename T>
typename quaternion<T>::this_type quaternion<T>::create_identity()
{
	return this_type(0, 0, 0, 1);
}

template <typename T>
quaternion<T>::quaternion(value_type x, value_type y, value_type z, value_type w) :
	m_x_(x),
	m_y_(y),
	m_z_(z),
	m_w_(w)
{
}

template <typename T>
quaternion<T>::quaternion(vec2_type v, value_type z, value_type w) :
	m_x_(v.get_x()),
	m_y_(v.get_y()),
	m_z_(z),
	m_w_(w)
{
}

template <typename T>
quaternion<T>::quaternion(value_type x, vec2_type v, value_type w) :
	m_x_(x),
	m_y_(v.get_x()),
	m_z_(v.get_y()),
	m_w_(w)
{
}

template <typename T>
quaternion<T>::quaternion(value_type x, value_type y, vec2_type v) :
	m_x_(x),
	m_y_(y),
	m_z_(v.get_x()),
	m_w_(v.get_y())
{
}

template <typename T>
quaternion<T>::quaternion(vec2_type v_0, vec2_type v_1) :
	m_x_(v_0.get_x()),
	m_y_(v_0.get_y()),
	m_z_(v_1.get_x()),
	m_w_(v_1.get_y())
{
}

template <typename T>
quaternion<T>::quaternion(value_type x, vec3_type v) :
	m_x_(x),
	m_y_(v.get_x()),
	m_z_(v.get_y()),
	m_w_(v.get_z())
{
}

template <typename T>
quaternion<T>::quaternion(vec3_type v, value_type w) :
	m_x_(v.get_x()),
	m_y_(v.get_y()),
	m_z_(v.get_z()),
	m_w_(w)
{
}

template <typename T>
quaternion<T>::quaternion(value_type v) :
	m_x_(v),
	m_y_(v),
	m_z_(v),
	m_w_(v)
{
}

template <typename T>
typename quaternion<T>::this_type quaternion<T>::operator*(const this_type& q) const
{
	vec3_type this_vec(m_x_, m_y_, m_z_);
	vec3_type q_vec(q.get_x(), q.get_y(), q.get_z());

	value_type r_w = m_w_ * q.get_w() + vec3_type::dot(this_vec, q_vec);
	vec3_type r_vec = this_vec * q.get_w() + q_vec * m_w_ + vec3_type::cross(this_vec, q_vec);

	return this_type(r_vec, r_w).normalized();
}

template <typename T>
typename quaternion<T>::this_type& quaternion<T>::operator*=(const this_type& q)
{
	vec3_type this_vec(m_x_, m_y_, m_z_);
	vec3_type q_vec(q.get_x(), q.get_y(), q.get_z());

	value_type r_w = m_w_ * q.get_w() + vec3_type::dot(this_vec, q_vec);
	vec3_type r_vec = this_vec * q.get_w() + q_vec * m_w_ + vec3_type::cross(this_vec, q_vec);

	m_x_ = r_vec.get_x();
	m_y_ = r_vec.get_y();
	m_z_ = r_vec.get_z();
	m_w_ = r_w;

	return *this->normalized();
}

template <typename T>
typename quaternion<T>::vec3_type quaternion<T>::operator*(const vec3_type& v) const
{
	this_type q(v, 0);
	vec3_type this_vec(m_x_, m_y_, m_z_);
	vec3_type vcv = vec3_type::cross(this_vec, v);
	return v + vcv * (2 * m_w_) + vec3_type::cross(this_vec, vcv) * 2;
}

template <typename T>
typename quaternion<T>::this_type quaternion<T>::inverse() const
{
	return this_type(m_x_ * -1, m_y_ * -1, m_z_ * -1, m_w_).normalized();
}

template <typename T>
typename quaternion<T>::float_type quaternion<T>::magnitude() const
{
	return math::sqrt(math::square(m_x_) + math::square(m_y_) + math::square(m_z_) + math::square(m_w_));
}

template <typename T>
typename quaternion<T>::this_type quaternion<T>::normalized() const
{
	float mag = 1 / magnitude();

	return this_type(m_x_ * mag, m_y_ * mag, m_z_ * mag, m_w_ * mag);
}
