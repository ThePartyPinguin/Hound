#pragma once

#include "hound/math/math_functions.h"

#include "hound/math/vector/vec2.h"
#include "hound/math/vector/vec3.h"
#include "hound/math/vector/vec4.h"

#include "hound/math/matrix/mat4.h"

#include "hound/math/quaternion/quaternion.h"

#include "hound/math/rect/rect.h"


typedef vec2<float> vec2_f;
typedef vec3<float> vec3_f;
typedef vec4<float> vec4_f;

typedef vec2<double> vec2_d;
typedef vec3<double> vec3_d;
typedef vec4<double> vec4_d;

typedef vec2<int> vec2_i;
typedef vec3<int> vec3_i;
typedef vec4<int> vec4_i;

typedef vec2<unsigned int> vec2_u;
typedef vec3<unsigned int> vec3_u;
typedef vec4<unsigned int> vec4_u;

typedef vec2<bool> vec2_b;
typedef vec3<bool> vec3_b;
typedef vec4<bool> vec4_b;

typedef mat4<float> mat4_f;
typedef mat4<double> mat4_d;
typedef mat4<int> mat4_i;
typedef mat4<bool> mat4_b;

typedef quaternion<float> quaternion_f;
typedef quaternion<double> quaternion_d;

typedef rect<float> rect_f;
typedef rect<double> rect_d;
typedef rect<int> rect_i;
