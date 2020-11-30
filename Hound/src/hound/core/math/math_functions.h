#pragma once

class math
{
public:
	constexpr  static float PI = 3.1415926535897932384626433832795f;
	constexpr  static float HALF_PI = 1.5707963267948966192313216916398f;
	constexpr  static float RAD_DEG_CONST = 57.295779513082320876798154814105f;
	constexpr  static float DEG_RAD_CONST = 0.01745329251994329576923690768489f;

	static float rad_to_deg(float angle_radians);
	static float deg_to_rad(float angle_degrees);

	static float sin(float angle);
	static float asin(float angle);

	static float cos(float angle);
	static float acos(float angle);

	static float tan(float angle);
	static float atan(float angle);
	static float atan2(float x, float y);

	static float sqrt(float value);
	static float square(float value);

	static float abs(float value);
	static float copy_sign(float mag, float sign);
};

