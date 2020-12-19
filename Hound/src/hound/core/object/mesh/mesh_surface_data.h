#pragma once
#include "hound/core/math/math.h"
#include <vector>

struct mesh_surface_data
{
	struct vertex
	{
		vec3_f position;
		vec3_f normal;
		vec2_f uv;
	};

	std::vector<vertex> vertices;
	std::vector<uint32_t> indices;
};