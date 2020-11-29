#include "hound/hnd_pch.h"
#include "math_functions.h"

float math::rad_to_deg(float angle_radians)
{
	return angle_radians * RAD_DEG_CONST;
}

float math::deg_to_rad(float angle_degrees)
{
	return angle_degrees * DEG_RAD_CONST;
}

float math::sin(float angle)
{
	return sinf(angle);
}

float math::asin(float angle)
{
	return asinf(angle);
}

float math::cos(float angle)
{
	return cosf(angle);
}

float math::acos(float angle)
{
	return acosf(angle);
}

float math::tan(float angle)
{
	return tanf(angle);
}

float math::atan(float angle)
{
	return atanf(angle);
}

float math::atan2(float x, float y)
{
	return atan2f(x, y);
}

float math::sqrt(float value)
{
	return sqrtf(value);
}

float math::square(float value)
{
	return value * value;
}

float math::abs(float value)
{
	return (value < 0) ? value * -1 : value;
}

float math::copy_sign(float mag, float sign)
{
	return std::copysign(mag, sign);
}
